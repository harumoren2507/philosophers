#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <string.h>
#include <errno.h>

/* カスタムセマフォの構造体 */
typedef struct s_semaphore {
    pthread_mutex_t mutex;      // 内部状態を保護するミューテックス
    pthread_mutex_t block;      // スレッドをブロックするためのミューテックス
    int value;                  // セマフォの値（利用可能なリソース数）
} t_semaphore;

/* フォークの状態を管理する構造体 */
typedef struct s_fork {
    pthread_mutex_t mutex;
} t_fork;

/* 哲学者の状態を管理する構造体 */
typedef struct s_philosopher {
    int id;                     // 哲学者の番号（1から始まる）
    int left_fork;              // 左フォークのインデックス 数？
    int right_fork;             // 右フォークのインデックス　数？
    int eating_count;           // 食事した回数
    long long last_meal_time;   // 最後の食事開始時刻
    pthread_mutex_t meal_mutex; // 最後の食事時間を保護するミューテックス
    struct s_data *data;        // 共有データへのポインタ
    pthread_t thread;           // 哲学者のスレッド
} t_philosopher;

/* 共有データを管理する構造体 */
typedef struct s_data {
    int n_philosophers;         // 哲学者の数
    int time_to_die;            // 死亡までの時間（ミリ秒）
    int time_to_eat;            // 食事にかかる時間（ミリ秒）
    int time_to_sleep;          // 睡眠にかかる時間（ミリ秒）
    int n_must_eat;             // 各哲学者が食べなければならない回数（オプション）
    int simulation_end;         // シミュレーション終了フラグ
    long long start_time;       // シミュレーション開始時刻
    t_fork *forks;              // フォークの配列
    t_philosopher *philosophers; // 哲学者の配列
    pthread_mutex_t print_mutex; // 出力を保護するミューテックス
    pthread_mutex_t end_mutex;   // 終了フラグを保護するミューテックス
    pthread_mutex_t meal_check_mutex; // 食事回数チェックを保護するミューテックス
    t_semaphore max_diners;     // 同時に食事できる哲学者の数を制限するセマフォ
} t_data;

int allocate_resources(t_data *data);
void cleanup_resources(t_data *data);

/* セマフォの初期化 */
void sem_custom_init(t_semaphore *sem, int initial_value)
{
    pthread_mutex_init(&sem->mutex, NULL);
    sem->value = initial_value;
}

void sem_custom_destroy(t_semaphore *sem)
{
    pthread_mutex_destroy(&sem->mutex);
}

void sem_custom_wait(t_semaphore *sem)
{
    while (1)
    {
        pthread_mutex_lock(&sem->mutex);
        if (sem->value > 0)
        {
            sem->value--;
            pthread_mutex_unlock(&sem->mutex);
            return;
        }
        pthread_mutex_unlock(&sem->mutex);
        usleep(1000);
    }
}

void sem_custom_post(t_semaphore *sem)
{
    pthread_mutex_lock(&sem->mutex);
    sem->value++;
    pthread_mutex_unlock(&sem->mutex);
}

/* 現在時刻をミリ秒で取得 */
long long get_time_ms(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

/* 指定されたミリ秒だけ高精度に待機 */
void precise_sleep(int ms)
{
    long long start;
    long long current;

    start = get_time_ms();
    while (1)
    {
        current = get_time_ms();
        if (current - start >= ms)
            break;
        usleep(100); // 短い間隔でチェック（高負荷を避けるため100μs）
    }
}

/* 状態変化のメッセージを出力 */
void print_status(t_philosopher *philo, char *message)
{
    t_data *data;
    long long current_time;
    int end;

    data = philo->data;
    pthread_mutex_lock(&data->end_mutex);
    end = data->simulation_end;
    pthread_mutex_unlock(&data->end_mutex);

    if (!end)
    {
        current_time = get_time_ms() - data->start_time;
        pthread_mutex_lock(&data->print_mutex);
        printf("%lld %d %s\n", current_time, philo->id, message);
        pthread_mutex_unlock(&data->print_mutex);
    }
}

/* シミュレーションが終了したかチェック */
int simulation_is_over(t_data *data)
{
    int result;

    pthread_mutex_lock(&data->end_mutex);
    result = data->simulation_end;
    pthread_mutex_unlock(&data->end_mutex);

    return (result);
}

/* 哲学者の最後の食事時間を更新 */
void update_last_meal_time(t_philosopher *philo)
{
    pthread_mutex_lock(&philo->meal_mutex);
    philo->last_meal_time = get_time_ms();
    pthread_mutex_unlock(&philo->meal_mutex);
}

/* 哲学者の最後の食事時間を取得 */
long long get_last_meal_time(t_philosopher *philo)
{
    long long time;

    pthread_mutex_lock(&philo->meal_mutex);
    time = philo->last_meal_time;
    pthread_mutex_unlock(&philo->meal_mutex);

    return time;
}

/* フォークを取る関数 */
void take_forks(t_philosopher *philo)
{
    t_data *data = philo->data;
    
    // 奇数と偶数でフォークを取る順番を変える（デッドロック防止）
    if (philo->id % 2 == 0)
    {
        pthread_mutex_lock(&data->forks[philo->left_fork].mutex);
        print_status(philo, "has taken a fork");
        pthread_mutex_lock(&data->forks[philo->right_fork].mutex);
        print_status(philo, "has taken a fork");
    }
    else
    {
        pthread_mutex_lock(&data->forks[philo->right_fork].mutex);
        print_status(philo, "has taken a fork");
        pthread_mutex_lock(&data->forks[philo->left_fork].mutex);
        print_status(philo, "has taken a fork");
    }
}

/* フォークを置く関数 */
void put_forks(t_philosopher *philo)
{
    t_data *data = philo->data;
    
    pthread_mutex_unlock(&data->forks[philo->left_fork].mutex);
    pthread_mutex_unlock(&data->forks[philo->right_fork].mutex);
}

/* 食事を行う関数 */
void eat(t_philosopher *philo)
{
    t_data *data = philo->data;
    
    if (!simulation_is_over(data))
    {
        print_status(philo, "is eating");
        update_last_meal_time(philo);
        precise_sleep(data->time_to_eat);

        pthread_mutex_lock(&data->meal_check_mutex);
        philo->eating_count++;
        pthread_mutex_unlock(&data->meal_check_mutex);
    }
}

/* 1人の哲学者の場合の特別処理 */
void *single_philosopher_routine(t_philosopher *philo)
{
    t_data *data = philo->data;
    
    print_status(philo, "is thinking");
    pthread_mutex_lock(&data->forks[philo->left_fork].mutex);
    print_status(philo, "has taken a fork");

    // 2本目のフォークが取れないので待機するだけ（死亡するまで）
    while (!simulation_is_over(data))
        precise_sleep(1);

    pthread_mutex_unlock(&data->forks[philo->left_fork].mutex);
    return NULL;
}

/* 哲学者のメインサイクル関数 */
void philosopher_cycle(t_philosopher *philo)
{
    t_data *data = philo->data;
    
    // 思考
    print_status(philo, "is thinking");

    // 同時に食事できる哲学者数を制限（セマフォを使用）
    sem_custom_wait(&data->max_diners);

    // フォークの取得
    take_forks(philo);

    // 食事
    eat(philo);

    // フォークを解放
    put_forks(philo);

    // セマフォを解放（他の哲学者が食事できるように）
    sem_custom_post(&data->max_diners);

    // 睡眠（シミュレーション終了チェック付き）
    if (!simulation_is_over(data))
    {
        print_status(philo, "is sleeping");
        precise_sleep(data->time_to_sleep);
    }
}

/* 哲学者のルーチン */
void *philosopher_routine(void *arg)
{
    t_philosopher *philo;
    t_data *data;

    philo = (t_philosopher *)arg;
    data = philo->data;

    // 1人の哲学者の場合は特別な処理
    if (data->n_philosophers == 1)
        return single_philosopher_routine(philo);

    if (philo->id % 2 == 0)
        usleep(5000);
    // 奇数番号の哲学者は少し遅らせてスタート（デッドロック防止の一助）
    /*
    if (philo->id % 2 != 0)
        precise_sleep(data->time_to_eat / 2);
    */
    // メインループ
    while (!simulation_is_over(data))
    {
        philosopher_cycle(philo);
    }

    return (NULL);
}

/* 全ての哲学者が十分に食事したかチェック */
int all_ate_enough(t_data *data)
{
    int i;
    int result = 1;

    if (data->n_must_eat < 0)
        return 0; // 無限に食べ続ける設定の場合

    pthread_mutex_lock(&data->meal_check_mutex);
    i = 0;
    while (i < data->n_philosophers)
    {
        if (data->philosophers[i].eating_count < data->n_must_eat)
        {
            result = 0;
            break;
        }
        i++;
    }
    pthread_mutex_unlock(&data->meal_check_mutex);

    return result;
}

/* 死亡したフィロソファーをチェック */
int check_philosopher_death(t_data *data, int i)
{
    long long current_time = get_time_ms();
    long long last_meal = get_last_meal_time(&data->philosophers[i]);
    
    // 最後の食事から time_to_die ミリ秒経過したかチェック
    if (current_time - last_meal > data->time_to_die)
    {
        print_status(&data->philosophers[i], "died");

        pthread_mutex_lock(&data->end_mutex);
        data->simulation_end = 1;
        pthread_mutex_unlock(&data->end_mutex);
        return 1;
    }
    return 0;
}

/* 哲学者の死亡監視のループ処理の修正版 */
void death_monitor_loop(t_data *data)
{
    int i;
    
    usleep(5000);
    while (!simulation_is_over(data))
    {
        i = 0;
        while (i < data->n_philosophers && !simulation_is_over(data))
        {
            if (check_philosopher_death(data, i))
                break;
            i++;
        }
        
        // 全員が必要回数食事したかチェック
        if (all_ate_enough(data) && !simulation_is_over(data))
        {
            pthread_mutex_lock(&data->end_mutex);
            data->simulation_end = 1;
            pthread_mutex_unlock(&data->end_mutex);
            pthread_mutex_lock(&data->print_mutex);
            printf("All philosophers have eaten enough.\n");
            pthread_mutex_unlock(&data->print_mutex);
            break;
        }
        usleep(1000);
    }
}

/* 哲学者の死亡監視 */
void *death_monitor(void *arg)
{
    t_data *data = (t_data *)arg;

    // 少し待機してから開始（哲学者スレッドが始まるのを待つ）
    precise_sleep(1);
    
    death_monitor_loop(data);
    
    return (NULL);
}

/* シミュレーションの初期化 */
int init_simulation(t_data *data, int argc, char **argv)
{
    memset(data, 0, sizeof(t_data));

    // コマンドライン引数の解析
    data->n_philosophers = atoi(argv[1]);
    data->time_to_die = atoi(argv[2]);
    data->time_to_eat = atoi(argv[3]);
    data->time_to_sleep = atoi(argv[4]);
    data->simulation_end = 0;

    if (argc == 6)
        data->n_must_eat = atoi(argv[5]);
    else
        data->n_must_eat = -1; // 無限に食べ続ける

    // 引数の妥当性チェック
    if (data->n_philosophers <= 0 || data->time_to_die < 60 ||
        data->time_to_eat < 60 || data->time_to_sleep < 60 ||
        (argc == 6 && data->n_must_eat <= 0))
        return 1;
    
    return allocate_resources(data);
}

/* リソースの割り当て */
int allocate_resources(t_data *data)
{
    // フォークの割り当て
    int max_diners;

    max_diners = data->n_philosophers -1;
    data->forks = malloc(sizeof(t_fork) * data->n_philosophers);
    if (!data->forks)
        return 1;

    // 哲学者の割り当て
    data->philosophers = malloc(sizeof(t_philosopher) * data->n_philosophers);
    if (!data->philosophers)
    {
        free(data->forks);
        return 1;
    }

    // ミューテックスの初期化
    pthread_mutex_init(&data->print_mutex, NULL);
    pthread_mutex_init(&data->end_mutex, NULL);
    pthread_mutex_init(&data->meal_check_mutex, NULL);

    if (max_diners < 1)
        max_diners = 1;
    // カスタムセマフォの初期化（最大N-1人が同時に食事可能）
    sem_custom_init(&data->max_diners, max_diners);

    return 0;
}

/* フォークの初期化 */
void init_forks(t_data *data)
{
    int i;

    i = 0;
    while (i < data->n_philosophers)
    {
        pthread_mutex_init(&data->forks[i].mutex, NULL);
        i++;
    }
}

/* 哲学者の初期化 */
void init_philosophers(t_data *data)
{
    int i;

    i = 0;
    while (i < data->n_philosophers)
    {
        data->philosophers[i].id = i + 1;
        data->philosophers[i].left_fork = i;
        data->philosophers[i].right_fork = (i + 1) % data->n_philosophers;
        data->philosophers[i].eating_count = 0;
        //data->philosophers[i].last_meal_time = get_time_ms();
        data->philosophers[i].data = data;
        pthread_mutex_init(&data->philosophers[i].meal_mutex, NULL);
        i++;
    }
}

/* シミュレーションの終了処理の修正版 */
void end_simulation(t_data *data)
{
    int i;

    // シミュレーション終了フラグを確実にセット
    pthread_mutex_lock(&data->end_mutex);
    data->simulation_end = 1;
    pthread_mutex_unlock(&data->end_mutex);
    
    // セマフォで待機中のスレッドを解放
    i = 0;
    while (i < data->n_philosophers)
    {
        sem_custom_post(&data->max_diners);
        usleep(500); // スレッドがセマフォを処理する時間を確保
        i++;
    }
    
    // スレッドの終了を待つ
    i = 0;
    while (i < data->n_philosophers)
    {
        pthread_join(data->philosophers[i].thread, NULL);
        i++;
    }
    
    // リソースのクリーンアップと解放
    cleanup_resources(data);
    free(data->forks);
    free(data->philosophers);
}

/* リソースのクリーンアップの修正版 */
void cleanup_resources(t_data *data)
{
    int i;
    
    // 各哲学者のミューテックスを解放
    i = 0;
    while (i < data->n_philosophers)
    {
        pthread_mutex_destroy(&data->forks[i].mutex);
        pthread_mutex_destroy(&data->philosophers[i].meal_mutex);
        i++;
    }
    
    // 共有ミューテックスを解放
    pthread_mutex_destroy(&data->print_mutex);
    pthread_mutex_destroy(&data->end_mutex);
    pthread_mutex_destroy(&data->meal_check_mutex);
    
    // 最後にセマフォを破棄
    sem_custom_destroy(&data->max_diners);
}

/* 哲学者スレッドの作成 */
int create_philosopher_threads(t_data *data)
{
    int i = 0;
    
    data->start_time = get_time_ms() + 10;
    while (i < data->n_philosophers)
    {
        //read_mutex_lock(&data->philosophers[i].meal_mutex);
        data->philosophers[i].last_meal_time = data->start_time;
        //hread_mutex_unlock(&data->philosophers[i].meal_mutex);
        i++;
    }
    i = 0;
    while (i < data->n_philosophers)
    {
        if (pthread_create(&data->philosophers[i].thread, NULL,
                          philosopher_routine, &data->philosophers[i]))
        {
            return 1;
        }
        i++;
    }
    
    return 0;
}

int main(int argc, char **argv)
{
    t_data data;
    pthread_t monitor_thread;

    // 引数チェック
    if (argc != 5 && argc != 6)
    {
        printf("Usage: %s number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n", argv[0]);
        return 1;
    }

    // シミュレーションの初期化
    if (init_simulation(&data, argc, argv))
    {
        printf("Error: invalid arguments or initialization failed\n");
        return 1;
    }

    // フォークと哲学者の初期化sem_custom_init
    init_forks(&data);
    init_philosophers(&data);

    // シミュレーション開始時刻を記録
    //data.start_time = get_time_ms();

    // 哲学者スレッドの作成
    if (create_philosopher_threads(&data))
    {
        printf("Error: thread creation failed\n");
        return 1;
    }

    // 監視スレッドの作成
    if (pthread_create(&monitor_thread, NULL, death_monitor, &data))
    {
        printf("Error: monitor thread creation failed\n");
        return 1;
    }

    // 監視スレッドの終了を待つ
    pthread_join(monitor_thread, NULL);

    // シミュレーションの終了処理
    end_simulation(&data);

    return 0;
}
