#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <string.h>
#include <queue>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>

using namespace std;

mutex mtx;

int num_producers = 2;
int num_consumers = 2;

int hour_index = 48;
int producer_count = 0;
int consumer_count = 0;
int total_rows = 0;

condition_variable producer_cv, consumer_cv;

string index_str, timestamp_str, traffic_light_id_str, num_of_cars_str;
vector<int> index_vec;
vector<int> traffic_light_vec;
vector<int> num_cars_vec;
vector<string> timestamps_vec;

struct TrafficSignal
{
    int index;
    std::string timestamp;
    int traffic_id;
    int num_of_cars;
};

TrafficSignal traffic_signals[4] = {{0, "", 1, 0}, {0, "", 2, 0}, {0, "", 3, 0}, {0, "", 4, 0}};

queue<TrafficSignal> traffic_signal_queue;
TrafficSignal signal;

bool sortMethod(struct TrafficSignal first, struct TrafficSignal second)
{
    if (first.num_of_cars > second.num_of_cars)
        return 1;
    return 0;
}

void *produceData(void *args)
{
    while (producer_count < total_rows)
    {
        unique_lock<mutex> lock(mtx);

        if (producer_count < total_rows)
        {
            traffic_signal_queue.push(TrafficSignal{index_vec[producer_count], timestamps_vec[producer_count], traffic_light_vec[producer_count], num_cars_vec[producer_count]});
            consumer_cv.notify_all();
            producer_count++;
        }

        else
        {
            producer_cv.wait(lock, []{ return producer_count < total_rows; });
        }

        lock.unlock();
        sleep(rand() % 3);
    }
}

void *consumeData(void *args)
{
    while (consumer_count < total_rows)
    {
        unique_lock<mutex> lock(mtx);

        if (!traffic_signal_queue.empty())
        {
            signal = traffic_signal_queue.front();

            if (signal.traffic_id == 1)
            {
                traffic_signals[0].num_of_cars += signal.num_of_cars;
            }
            if (signal.traffic_id == 2)
            {
                traffic_signals[1].num_of_cars += signal.num_of_cars;
            }
            if (signal.traffic_id == 3)
            {
                traffic_signals[2].num_of_cars += signal.num_of_cars;
            }
            if (signal.traffic_id == 4)
            {
                traffic_signals[3].num_of_cars += signal.num_of_cars;
            }

            traffic_signal_queue.pop();
            producer_cv.notify_all();
            consumer_count++;
        }
        else
        {
            consumer_cv.wait(lock, []{ return !traffic_signal_queue.empty(); });
        }

        if (consumer_count % hour_index == 0)
        {
            sort(traffic_signals, traffic_signals + 4, sortMethod);
            printf("Traffic lights sorted according to busyness | Time: %s \n", signal.timestamp.c_str());
            cout << "Traffic Light"<< "\t"<< "Number of Cars" << endl;
            for (int i = 0; i < 4; i++)
            {
                cout << traffic_signals[i].traffic_id << "\t"<< "\t" << traffic_signals[i].num_of_cars << endl;
            }
        }

        lock.unlock();
        sleep(rand() % 3);
    }
}

void readTrafficData()
{
    ifstream infile;

    string filename;
    cout << "Enter the filename: ";
    cin >> filename;

    infile.open(filename);

    if (infile.is_open())
    {
        std::string line;
        getline(infile, line);

        while (!infile.eof())
        {
            getline(infile, index_str, ',');
            index_vec.push_back(stoi(index_str));
            getline(infile, timestamp_str, ',');
            timestamps_vec.push_back(timestamp_str);
            getline(infile, traffic_light_id_str, ',');
            traffic_light_vec.push_back(stoi(traffic_light_id_str));
            getline(infile, num_of_cars_str, '\n');
            num_cars_vec.push_back(stoi(num_of_cars_str));

            total_rows += 1;
        }
        infile.close();
    }
    else
        printf("Could not open file, try again.");
}

int main()
{

    readTrafficData();

    pthread_t producers[num_producers];
    pthread_t consumers[num_consumers];

    for (long i = 0; i < num_producers; i++)
        pthread_create(&producers[i], NULL, produceData, (void *)i);
    for (long i = 0; i < num_consumers; i++)
        pthread_create(&consumers[i], NULL, consumeData, (void *)i);

    for (long i = 0; i < num_producers; i++)
        pthread_join(producers[i], NULL);
    for (long i = 0; i < num_consumers; i++)
        pthread_join(consumers[i], NULL);
}
