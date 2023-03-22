from flask import Flask, jsonify, request, render_template
import json
import socket
from flask_cors import CORS
import pandas as pd
from scipy.spatial import KDTree
import numpy as np
# from flask_sqlalchemy import SQLAlchemy

app = Flask(__name__)
CORS(app, supports_credentials=True)

vertex_number = 0
id_coordinate = []
vertex_key = []
key_string = []

def read_city_coordinate(city_file):
    global vertex_number
    global id_coordinate
    nyFile = open(city_file)
    lines = nyFile.readlines()
    vertex_number = int(lines[0][:-1])
    # print("vertex_number = ", vertex_number)
    id_coordinate = [[] for i in range(vertex_number)]
    # print(lines[0][:-1].split(" "))
    for line in lines[1:]:
        items = line[:-1].split(" ")
        id_coordinate[int(items[1])-1] = [int(items[2])/1000000, int(items[3])/1000000]
    # print("The city coordinate file has been read (%d items)" %(len(id_coordinate)))

def read_vertex_key(vk_file):
    global vertex_key
    vertex_key = [[] for i in range(vertex_number)]
    # print("len : ", vertex_number, len(vertex_key))
    nyFile = open(vk_file)
    lines = nyFile.readlines()
    for line in lines[1:]:
        items = line[:-1].split(" ")
        v = int(items[0])
        # print(v)
        if len(items) < 3:
            continue
        for k in items[2:]:
            vertex_key[v].append(int(k))

def read_key_string(ks_file):
    nyFile = open(ks_file)
    lines = nyFile.readlines()
    for line in lines:
        key_string.append(line[:-1].capitalize())

def read_city_info(city):
    read_city_coordinate("../TASK/datasets/"+city+"/"+city+".co")
    read_vertex_key("../TASK/datasets/"+city+"/"+city+".vertexKey")
    read_key_string("../TASK/datasets/"+city+"/"+city+".string")
    print("city information has been read (%d %d %d)" \
            %(len(id_coordinate), len(vertex_key), len(key_string)))

def generate_kdtree():
    np_id_co = np.array(id_coordinate)
    df = pd.DataFrame(np_id_co, columns=['longitude', 'latitude'])
    df_coords = df[['longitude', 'latitude']].values
    tree = KDTree(df_coords)
    return tree

def get_position_id(long, lat):
    global tree
    dist, idx = tree.query([long, lat], k=1)
    return idx

@app.route('/')
def index():
    return 'Index Page'

@app.route('/hello')
def hello():
    return 'Hello, World'

@app.route('/locToPlace', methods=['GET','post'])
def locToPlace():
    data = request.get_json(silent=True)
    # print(type(data), data)
    id = get_position_id(data["longitude"], data["latitude"])
    name = ""
    if len(vertex_key[id])>1:
        name = key_string[vertex_key[id][0]]
        for k in vertex_key[id][1:]:
            name = name + " " + key_string[k]
    data = {
        'id':str(id),\
        'name':name,\
        'longitude':id_coordinate[id][1],\
        'latitude':id_coordinate[id][0],\
    }
    # print(data)
    return jsonify({'place':data})

@app.route('/queryPoints', methods=['GET','post'])
def queryPoints():
    print("queryPoints start...")
    # get data from html frontend
    data = request.get_json(silent=True)
    print("data from queryPoints", type(data), data)
    # print(data['name'], data["longitude"], data["latitude"])

    # get the nearest point in map center
    center_postion_idx = get_position_id(data["longitude"], data["latitude"])

    # set socket
    address = ('127.0.0.1', 1234)
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind(address)
    server.listen(5)
    client, addr = server.accept()
    
    # send query data to c++ backend
    data = data['k']+','+data['tau']+','+data['alpha']+','+str(center_postion_idx)+','+data['name']
    data = "x"+str(len(data))+','+data  # x means queryPoints
    # data = str(len(data))+','+data  # x means queryPoints
    print("data to c++ in flask : ", data)
    client.send(data.encode('utf-8'))

    # receive return data from c++ backend, process and return
    res = client.recv(1024)
    print("res from IQA : ", type(res), res)
    place_ids_str = str(res, 'utf-8')

    data = []

    if place_ids_str == 'NORESULT':
        server.close()
        return jsonify({'places':data})
    
    print("place_ids_str", place_ids_str)
    info_time = place_ids_str.split("+")
    print("info_time", info_time)
    
    place_ids_str = info_time[0]
    place_ids_str = place_ids_str.split(",")
    place_id_scores = [x.split("-") for x in place_ids_str]
    place_id_scores = [(int(id_score[0]), float(id_score[1])) for id_score in place_id_scores]
    print("place_id_scores", place_id_scores)

    for pid_score in place_id_scores:
        pid = pid_score[0]
        score = pid_score[1]
        name = ""
        if len(vertex_key[pid])>1:
            name = key_string[vertex_key[pid][0]]
            for k in vertex_key[pid][1:]:
                name = name + " " + key_string[k]
        data.append({\
            'id':str(pid),\
            'score':score,\
            'name':name,\
            'time':float(info_time[1]),\
            'longitude':id_coordinate[pid][1],\
            'latitude':id_coordinate[pid][0],\
        })
    print(data)

    server.close()
    return jsonify({'places':data})

@app.route('/queryPath', methods=['GET','post'])
def queryPath():
    print("queryPath start...")
    # get data from html frontend
    data = request.get_json(silent=True)
    print("data from queryPath", data)
    # print(data["startId"], data["endId"])

    # set socket
    address = ('127.0.0.1', 1234)
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind(address)
    server.listen(5)
    client, addr = server.accept()
    
    # send query data to c++ backend
    data = data["startId"]+','+data["endId"]
    data = "p"+str(len(data))+','+data
    print("data to c++ in flask : ", data)
    client.send(data.encode('utf-8'))

    # receive return data from c++ backend, process and return
    res = client.recv(1024)
    print("res from IQA : ", type(res), res)
    path_ids_str = str(res, 'utf-8')

    data = []

    if path_ids_str == 'NORESULT':
        server.close()
        return jsonify({'places':data})

    print("path_ids_str", path_ids_str)
    info_time = path_ids_str.split("+")
    print("info_time", info_time)
    path_ids_str = info_time[0]

    path_ids_str = path_ids_str.split(",")
    path_ids = [int(x) for x in path_ids_str]

    for pid in path_ids:
        name = ""
        if len(vertex_key[pid])>1:
            name = key_string[vertex_key[pid][0]]
            for k in vertex_key[pid][1:]:
                name = name + " " + key_string[k]
        data.append({\
            'id':pid,\
            'name':name,\
            'time':float(info_time[1]),\
            'longitude':id_coordinate[pid][1],\
            'latitude':id_coordinate[pid][0],\
        })
    print(data)

    server.close()
    return jsonify({'path':data})

if __name__ == '__main__':
    read_city_info(city="NY")

    tree = generate_kdtree()

    app.run(host="0.0.0.0", port=5000, debug=True)
