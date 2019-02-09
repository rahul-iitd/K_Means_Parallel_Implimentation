//
// Created by Rahul Bansal on 2019-01-30.
//
#include <bits/stdc++.h>
#include <malloc.h>
#include <cmath>
#include <stdlib.h>
#include <stdio.h>
#include <iostream.h>
#include "lab1_sequential.h"
using namespace std;



class Point{
public:
    int x;
    int y;
    int z;

    int distance(Point a,int px,int py,int pz){
        return (a.x-px)*(a.x-px)+(a.y-py)*(a.y-py)+(a.z-pz)*(a.z-pz);
    }

};
vector<vector<Point>> centroid_iterations;
vector<Point> my_centroids;
vector<int> centroid_number_old;
vector<int> centroid_number_new;

int closest(int px,int py,int pz){
    int min = std::numeric_limits<int>::max();
    int min_index=0;
    for (int i = 0; i <my_centroids.size() ; ++i) {
        int d=Point::distance(my_centroids[i],px,py,pz);
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

void kmeans_sequential(int N,
                       int K,
                       int* data_points,
                       int** data_point_cluster,
                       int** centroids,
                       int* num_iterations){

    int size_of_partition= floor(N/K);

    for (int i = 0; i <K ; ++i) {
        Point p;
        p.x=data_points[0][i*size_of_partition];
        p.y=data_points[0][i*size_of_partition+1];
        p.z=data_points[0][i*size_of_partition+2];
        my_centroids.push_back(p);
    }

    int iteration=0;

    while (true){

        centroid_number_old.clear();
        centroid_number_old=centroid_number_new;

        for (int i = 0; i <3*N ; i=i+3) {
            centroid_number_new.push_back(closest(data_points[i],data_points[i+1],data_points[i+2]));
        }

        if (iteration!=0 and converge(centroid_number_new,centroid_number_old)) break;
        iteration++;
        vector<Point>new_centroids;
        vector<int>number_of_points_per_cluster(K,0);
        for (int i = 0; i < K; ++i) {
            Point p;
            p.x=0;p.y=0;p.z=0;
            new_centroids.push_back(p);
        }
        for (int i = 0; i < 3*N; i=i+3) {
            int c=centroid_number_new[i];
            number_of_points_per_cluster[c]+=1;
            new_centroids[c].x+=data_points[i];
            new_centroids[c].y+=data_points[i+1];
            new_centroids[c].z+=data_points[i+2];
        }
        for (int i = 0; i < K; ++i) {
            new_centroids[i].x=floor(new_centroids[i].x/number_of_points_per_cluster[i]);
            new_centroids[i].y=floor(new_centroids[i].y/number_of_points_per_cluster[i]);
            new_centroids[i].z=floor(new_centroids[i].z/number_of_points_per_cluster[i]);
        }
        centroid_iterations.push_back(my_centroids);
        my_centroids.clear();
        my_centroids=new_centroids;
    }

    *data_point_cluster= (int*) calloc(4*N, sizeof(int));
    *centroids = (int*) calloc(3*centroid_iterations.size()*K, sizeof(int));
    int a=0;
    for (int i = 0; i <centroid_iterations.size() ; ++i) {
        for (int j = 0; j <K ; ++j) {
            centroids[0][a]=centroid_iterations[i][j].x;
            centroids[0][a+1]=centroid_iterations[i][j].x;
            centroids[0][a+2]=centroid_iterations[i][j].x;
            a=a+3;
        }
    }
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

