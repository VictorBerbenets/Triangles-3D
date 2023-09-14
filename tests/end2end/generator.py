import Geometry3D as geometry
import sys

def create_triangles(file_name):
    trias_number, data = file_data(file_name)
    points_arr         = get_points(trias_number, data)

    triangles     = []
    points_number = len(points_arr)
    i = 0
    while((i + 2) != (points_number - 1)):
        triangles.append(geometry.ConvexPolygon(points_arr[i], points_arr[i + 1], points_arr[i + 2]))
        i += 3
    return triangles


def file_data(file_name):
    data = []
    with open(file_name) as f:
        for line in f:
            data.append([float(x) for x in line.split()])
    trias_number = data[0]
    data.pop(0)

    return trias_number, data

def get_points(trias_number, data):
    points_arr = []

    for i in range (0, trias_number):
        points_arr.append(geometry.Point(data[i], data[i + 1], data[i + 2]))
        i += 2
    return points_arr

def get_intersec_triangles(triangles):
    trias_len = len(triangles)
    intsec_indexes = []
    for i in range (0, trias_len):
        for j in range (i + 1, trias_len):
            if (geometry.intersection(triangles[i], triangles[j])):
                if i not in intsec_indexes:
                    intsec_indexes.append(i)
                if j not in intsec_indexes:
                    intsec_indexes.append(j)
    intsec_indexes.sort()
    return intsec_indexes

def generate_answ_file(intsec_trias):
    answ = open("")

def main ():
    intsec_trias = get_intersec_triangles(create_triangles(sys.argv[1]))
    generate_answ_file(intsec_trias)    

main()

