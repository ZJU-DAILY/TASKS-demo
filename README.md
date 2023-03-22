# TASKS

## Introduction
This repository contains the code used in our paper: **TASKS: A Real-time Query System for Instant Error-Tolerant Spatial Keyword Queries on Road Networks**. TASKS is a real-time query system for instant error-tolerant spatial keyword queries on road networks, which consists of an index component, a query component, an update component, and a route component.

## Running Environment

a 64-bit Linux-based OS; a windows OS

## Datasets
We use a publicly available real-world road networks of New York as an example, which is obtained from <font color='red'>**...**</font>

The data is shown in folder `demo-backend/TASK/datasets/NY`.

## Usage
#### How to run

- Step 1: Push the demo-backend part code to the linux server

- Step 2: Move to directory `demo/demo-backend` and run flask server, through which the data interaction between the front-end interface and the back-end TASK is realized

  ```cmd
  python demo_flask.py
  ```

- Step 3: Move to directory `demo/demo-backend/TASK` and run back-end TASK derived from [TASK](https://github.com/ZJU-DAILY/TASK)

  ```cmd
  make go
  ```

- Step 4: Open the front-end interface `/demo-frontend/TASKS-demo.html` at Windows  to use TASKS

## Requirements
+ cmake
+ g++
+ OpenMP
+ Boost
+ flask