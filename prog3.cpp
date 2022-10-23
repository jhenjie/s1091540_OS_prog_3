#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <pthread.h>
#include <string>
#include <utility>
#include <unistd.h>
#include <vector>

using namespace std;

pthread_mutex_t mutex;

int Mode = 0;
int Seed = 0;
int Case = 0;

int drones = 0;
int P1_drone = 0;
int P2_drone = 0;
int P3_drone = 0;

vector<int> accessories(3);
vector<int> P1_accessory(3);
vector<int> P2_accessory(3);
vector<int> P3_accessory(3);

vector<int> A_prepared(3);
vector<int> B_prepared(3);

void *Dispatcher_A(void *arg)
{
    while (drones < 50)
    {
        pthread_mutex_lock(&mutex);

        if (Mode == 0)
        {
            Case = rand() % 3;
        }
        else if (Mode == 1)
        {
            Case = rand() % 2;
        }

        switch (Case)
        {
        case 0:
            if (accessories[0] == 0)
            {
                accessories[0] = 1;
                A_prepared[0]++;

                if (Mode == 0)
                {
                    cout << "Dispatcher: battery\n";
                }
                else if (Mode == 1)
                {
                    cout << "Dispatcher A: battery\n";
                }
            }
            break;
        case 1:
            if (accessories[1] == 0)
            {
                accessories[1] = 1;
                A_prepared[1]++;

                if (Mode == 0)
                {
                    cout << "Dispatcher: aircraft\n";
                }
                else if (Mode == 1)
                {
                    cout << "Dispatcher A: aircraft\n";
                }
            }
            break;
        case 2:
            if (accessories[2] == 0)
            {
                accessories[2] = 1;
                A_prepared[2]++;
                cout << "Dispatcher: propeller\n";
            }
            break;
        default:
            break;
        }

        pthread_mutex_unlock(&mutex);

        sleep(0.1);
    }

    return NULL;
}

void *Dispatcher_B(void *arg)
{
    while (drones < 50)
    {
        pthread_mutex_lock(&mutex);

        Case = rand() % 2;

        switch (Case)
        {
        case 0:
            if (accessories[1] == 0)
            {
                accessories[1] = 1;
                B_prepared[1]++;
                cout << "Dispatcher B: aircraft\n";
            }
            break;
        case 1:
            if (accessories[2] == 0)
            {
                accessories[2] = 1;
                B_prepared[2]++;
                cout << "Dispatcher B: propeller\n";
            }
            break;
        default:
            break;
        }

        pthread_mutex_unlock(&mutex);

        sleep(0.1);
    }

    return NULL;
}

void *Producer_1(void *arg)
{
    while (drones < 50)
    {
        pthread_mutex_lock(&mutex);

        if (P1_accessory[0] == 0 && accessories[0] == 1)
        {
            P1_accessory[0] = 1;
            accessories[0] = 0;
            cout << "Producer 1 (aircraft): get battery\n";
        }
        else if (P1_accessory[2] == 0 && accessories[2] == 1)
        {
            P1_accessory[2] = 1;
            accessories[2] = 0;
            cout << "Producer 1 (aircraft): get propeller\n";
        }

        if (P1_accessory[0] == 1 && P1_accessory[2] == 1)
        {
            drones++;
            P1_drone++;
            P1_accessory[0] = 0;
            P1_accessory[2] = 0;
            cout << "Producer 1 (aircraft): OK, " << P1_drone << " drone(s)\n";
        }

        pthread_mutex_unlock(&mutex);

        sleep(0.1);
    }

    return NULL;
}

void *Producer_2(void *arg)
{
    while (drones < 50)
    {
        pthread_mutex_lock(&mutex);

        if (P2_accessory[0] == 0 && accessories[0] == 1)
        {
            P2_accessory[0] = 1;
            accessories[0] = 0;
            cout << "Producer 2: get battery\n";
        }
        else if (P2_accessory[1] == 0 && accessories[1] == 1)
        {
            P2_accessory[1] = 1;
            accessories[1] = 0;
            cout << "Producer 2: get aircraft\n";
        }
        else if (P2_accessory[2] == 0 && accessories[2] == 1)
        {
            P2_accessory[2] = 1;
            accessories[2] = 0;
            cout << "Producer 2: get propeller\n";
        }

        if (P2_accessory[0] == 1 && P2_accessory[1] == 1 && P2_accessory[2] == 1)
        {
            drones++;
            P2_drone++;
            P2_accessory[0] = 0;
            P2_accessory[1] = 0;
            P2_accessory[2] = 0;
            cout << "Producer 2: OK, " << P2_drone << " drone(s)\n";
        }

        pthread_mutex_unlock(&mutex);

        sleep(0.1);
    }

    return NULL;
}

void *Producer_3(void *arg)
{
    while (drones < 50)
    {
        pthread_mutex_lock(&mutex);

        if (P3_accessory[0] == 0 && accessories[0] == 1)
        {
            P3_accessory[0] = 1;
            accessories[0] = 0;
            cout << "Producer 3: get battery\n";
        }
        else if (P3_accessory[1] == 0 && accessories[1] == 1)
        {
            P3_accessory[1] = 1;
            accessories[1] = 0;
            cout << "Producer 3: get aircraft\n";
        }
        else if (P3_accessory[2] == 0 && accessories[2] == 1)
        {
            P3_accessory[2] = 1;
            accessories[2] = 0;
            cout << "Producer 3: get propeller\n";
        }

        if (P3_accessory[0] == 1 && P3_accessory[1] == 1 && P3_accessory[2] == 1)
        {
            drones++;
            P3_drone++;
            P3_accessory[0] = 0;
            P3_accessory[1] = 0;
            P3_accessory[2] = 0;
            cout << "Producer 3: OK, " << P3_drone << " drone(s)\n";
        }

        pthread_mutex_unlock(&mutex);

        sleep(0.1);
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    string buffer_1 = argv[1];
    Mode = stoi(buffer_1);

    string buffer2 = argv[2];
    Seed = stoi(buffer2);

    srand(Seed);

    if (Mode == 0)
    {
        int args[4] = {0};
        pthread_t threads[4];

        pthread_mutex_init(&mutex, NULL);

        pthread_create(&threads[0], NULL, Dispatcher_A, (void *)&args[0]);
        pthread_create(&threads[1], NULL, Producer_1, (void *)&args[1]);
        pthread_create(&threads[2], NULL, Producer_2, (void *)&args[2]);
        pthread_create(&threads[3], NULL, Producer_3, (void *)&args[3]);

        pthread_join(threads[0], NULL);
        pthread_join(threads[1], NULL);
        pthread_join(threads[2], NULL);
        pthread_join(threads[3], NULL);

        pthread_mutex_destroy(&mutex);

        vector<pair<int, string>> result = {{P1_drone, "Producer 1"}, {P2_drone, "Producer 2"}, {P3_drone, "Producer 3"}};
        sort(result.begin(), result.end());

        cout << "Dispatcher: " << A_prepared[0] << " batteries " << A_prepared[1] << " aircrafts " << A_prepared[2] << " propellers \n";

        for (int i = 2; i >= 0; i--)
        {
            cout << result[i].second << ": " << result[i].first << "\n";
        }
    }
    else if (Mode == 1)
    {
        int args[5] = {0};
        pthread_t threads[5];

        pthread_mutex_init(&mutex, NULL);

        pthread_create(&threads[0], NULL, Dispatcher_A, (void *)&args[0]);
        pthread_create(&threads[1], NULL, Dispatcher_B, (void *)&args[1]);
        pthread_create(&threads[2], NULL, Producer_1, (void *)&args[2]);
        pthread_create(&threads[3], NULL, Producer_2, (void *)&args[3]);
        pthread_create(&threads[4], NULL, Producer_3, (void *)&args[4]);

        pthread_join(threads[0], NULL);
        pthread_join(threads[1], NULL);
        pthread_join(threads[2], NULL);
        pthread_join(threads[3], NULL);
        pthread_join(threads[4], NULL);

        pthread_mutex_destroy(&mutex);

        vector<pair<int, string>> result = {{P1_drone, "Producer 1"}, {P2_drone, "Producer 2"}, {P3_drone, "Producer 3"}};
        sort(result.begin(), result.end());

        cout << "Dispatcher A: " << A_prepared[0] << " batteries " << A_prepared[1] << " aircrafts " << A_prepared[2] << " propellers \n";
        cout << "Dispatcher B: " << B_prepared[0] << " batteries " << B_prepared[1] << " aircrafts " << B_prepared[2] << " propellers \n";

        for (int i = 2; i >= 0; i--)
        {
            cout << result[i].second << ": " << result[i].first << "\n";
        }
    }

    return 0;
}