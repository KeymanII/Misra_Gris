import json
import networkx as nx
import random
import matplotlib.pyplot as plt


def original_check(num1, num2):
    range = 10000
    if (num1 <= num2 - range) or (num1 >= num2 + range):
        return True
    return False


def generate_colors(colors):
    string_colors = []
    used_colors = {}
    for color in colors:
        original = True
        color_string = random.randint(100000, 999999)
        for key in used_colors.keys():
                if not original_check(color_string, key):
                    while not original_check(color_string, key):
                        color_string = random.randint(100000, 999999)
        color = color_string
        string_colors.append(f"#{color}")
    return string_colors


with open("colors.json") as cl:
    #colors: [1, 2, {color: "some shi"}]
    colors = json.load(cl)
    nodes = set()
    edges = []
    for j in range(len(colors) - 1):
        edges.append(colors[j])
    colors_indeed = colors[-1]
    print(colors_indeed)
    normalized_colors = generate_colors(colors_indeed)
    print(normalized_colors)
    for i in range(0, len(edges)):
        nodes.add(edges[i][0])
        nodes.add(edges[i][1])
        edges[i].extend([{'color': normalized_colors[i], 'weight': 8}])

    print(edges, colors_indeed)
    graph = nx.Graph()
    graph.add_nodes_from(nodes)
    graph.add_edges_from(edges)
    pos = nx.spring_layout(graph)
    nx.draw_networkx_nodes(graph, node_size=900, node_color="gray", pos=pos)  # Узлы
    nx.draw_networkx_edges(graph, width=6, edge_color=normalized_colors, pos=pos)  # Ребра
    nx.draw_networkx_labels(graph, font_size=12, font_color="white", pos=pos)
    plt.show()


    cl.close()
