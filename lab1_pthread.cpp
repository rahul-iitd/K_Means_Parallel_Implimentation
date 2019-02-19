//
// Created by Rahul Bansal on 2019-01-30.
//
#include <bits/stdc++.h>
#include <malloc.h>
#include <cmath>
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include "lab1_pthread.h"
#include <pthread.h>
using namespace std;


int N;
int* data_points;
int num_threads;
int* c_number;

class Point{
public:
    float x;
    float y;
    float z;

    float distance(int px,int py,int pz){
        return (x-px)*(x-px)+(y-py)*(y-py)+(z-pz)*(z-pz);
    }

};
vector<vector<Point>> centroid_iterations;
vector<Point> my_centroids;
vector<int> centroid_number_new;

int closest(int px,int py,int pz){
    int min = std::numeric_limits<int>::max();
    int min_index=0;
    for (int i = 0; i <my_centroids.size() ; ++i) {
        int d=my_centroids[i].distance(px,py,pz);
        if (d<=min){
            min=d;
            min_index=i;
        }
    }
    return min_index;
}

bool converge(vector<int>v1, vector<int>v2){
    for (int i = 0; i < v1.size(); ++i) {
        if (v1[i]!=v2[i]) return false;
    }
    return true;
}

void* count3s_thread(void *tid)
{

  int *id = (int*) tid;
  int length_per_thread = 3*N/num_threads;

  int start = (*id)*length_per_thread;

  int i;
  if (*id!=num_threads-1){
  for (int i = start; i <start+length_per_thread ; i=i+3) {
      c_number[i/3]=closest(data_points[i],data_points[i+1],data_points[i+2]);
      // centroid_number_new.push_back(closest(data_points[i],data_points[i+1],data_points[i+2]));
  }
}
  if (*id==num_threads-1){
    for (int i = start; i <3*N ; i=i+3) {
        c_number[i/3]=closest(data_points[i],data_points[i+1],data_points[i+2]);
        // centroid_number_new.push_back(closest(data_points[i],data_points[i+1],data_points[i+2]));
    }
  }
  return NULL;
}

void kmeans_pthread(int num_threads,
					int N,
					int K,
					int* data_points,
					int** data_point_cluster,
					float** centroids,
					int* num_iterations
					){

    ::num_threads=num_threads;
    ::N=N;
    ::data_points=data_points;
    ::c_number=c_number;
    pthread_t count3s_thr[num_threads];
    int size_of_partition= floor(N/(2*K));
    srand(1);
    for (int i = 0; i <K ; ++i) {
        Point p;
        int a=rand()%N;
        p.x=data_points[3*a];
        p.y=data_points[3*a+1];
        p.z=data_points[3*a+2];
        my_centroids.push_back(p);
    }
    for (int i = 0; i <3*N ; i=i+3) {
        centroid_number_new.push_back(closest(data_points[i],data_points[i+1],data_points[i+2]));
    }

    int iteration=0;
    while (true){
        c_number = (int *) malloc (sizeof (int) * N);
        iteration++;
        // std::cout << iteration << '\n';
        vector<int> centroid_number_old;
        centroid_number_old=centroid_number_new;
        centroid_number_new.clear();

        vector<Point>new_centroids;
        vector<int>number_of_points_per_cluster(K,0);
        for (int i = 0; i < K; ++i) {
            Point p;
            p.x=0;p.y=0;p.z=0;
            new_centroids.push_back(p);
        }
        // std::cout << "1" << '\n';
        for (int i = 0; i < 3*N; i=i+3) {
            int c=centroid_number_old[i/3];
            number_of_points_per_cluster[c]+=1;
            new_centroids[c].x+=data_points[i];
            new_centroids[c].y+=data_points[i+1];
            new_centroids[c].z+=data_points[i+2];
        }
        // std::cout << "2" << '\n';
        for (int i = 0; i < K; ++i) {

            new_centroids[i].x=(new_centroids[i].x/number_of_points_per_cluster[i]);
            new_centroids[i].y=(new_centroids[i].y/number_of_points_per_cluster[i]);
            new_centroids[i].z=(new_centroids[i].z/number_of_points_per_cluster[i]);
        }
        // std::cout << "3" << '\n';
        centroid_iterations.push_back(my_centroids);
        my_centroids.clear();
        my_centroids=new_centroids;

        int *tid = (int *) malloc (sizeof (int) * num_threads);
        for (int i = 0; i < num_threads; i ++) {
    			tid[i] = i;
    			pthread_create(&count3s_thr[i], NULL, count3s_thread, &tid[i]);
        }
        for (int i = 0; i <num_threads; i ++){
          pthread_join(count3s_thr[i], NULL);
        }
        // std::cout << "4" << '\n';
        for (int i = 0; i < N; i++) {
            centroid_number_new.push_back(c_number[i]);
        }

        if (converge(centroid_number_new,centroid_number_old)) break;
    }

    std::vector<float> centroids_vector;

    for (int i=0;i<centroid_iterations.size();i++){
      for (int j = 0; j < K; j++) {
        centroids_vector.push_back(centroid_iterations[i][j].x);
        centroids_vector.push_back(centroid_iterations[i][j].y);
        centroids_vector.push_back(centroid_iterations[i][j].z);
      }
    }

    *data_point_cluster= (int*) calloc(4*N, sizeof(int));
    *centroids = (float*) calloc(centroids_vector.size(), sizeof(float));
    int a=0;
    // std::cout << centroid_iterations.size() << '\n';
    for (int i = 0; i <centroids_vector.size() ;i++) {
        centroids[0][i]=centroids_vector[i];
    }
    *num_iterations=centroid_iterations.size()-1;
    int b=0;
    for (int i = 0; i <3*N ; i=i+3) {
        int c_id=i/3;
        data_point_cluster[0][b]=data_points[i];
        data_point_cluster[0][b+1]=data_points[i+1];
        data_point_cluster[0][b+2]=data_points[i+2];
        data_point_cluster[0][b+3]=centroid_number_new[c_id];
        b=b+4;
    }
}
