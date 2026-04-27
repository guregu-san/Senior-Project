import csv
import networkx as nx
import matplotlib.pyplot as plt

def load_graph_from_csv(file_path):
    G = nx.Graph()

    with open(file_path, 'r') as f:
        reader = csv.DictReader(f)
        for row in reader:
            node = row['Node']
            neighbors = row['Neighbours']

            G.add_node(node)

            if neighbors.strip():
                for neighbor in neighbors.split(','):
                    neighbor = neighbor.strip()
                    G.add_edge(node, neighbor)

    return G

def draw_graph(G):
    pos = nx.spring_layout(G)
    nx.draw(
        G, pos,
        with_labels=True,
        node_color='lightblue',
        edge_color='gray',
        node_size=1500,
        font_size=10
    )
    plt.title("Network Graph")
    plt.show(block=True)

if __name__ == "__main__":
    file_path = "log.csv"
    graph = load_graph_from_csv(file_path)
    draw_graph(graph)
