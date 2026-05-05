import csv
import networkx as nx
import matplotlib.pyplot as plt

def load_graphs_from_csv(file_path):
    original = nx.Graph()
    reduced = nx.Graph()

    # Check which nodes got deleted
    nodes = {}

    with open(file_path, 'r') as f:
        reader = csv.DictReader(f)

        for row in reader:
            node = row['Node']
            deleted = row['Deleted'] == "1"
            neighbors = row['Neighbours']

            nodes[node] = {
                "deleted": deleted,
                "neighbors": []
            }

            original.add_node(node)

    with open(file_path, 'r') as f:
        reader = csv.DictReader(f)

        for row in reader:
            node = row['Node']
            neighbors = row['Neighbours']

            if neighbors.strip():
                for neighbor in neighbors.split(','):
                    neighbor = neighbor.strip()

                    original.add_edge(node, neighbor)

                    if not nodes[node]["deleted"] and not nodes[neighbor]["deleted"]:
                        reduced.add_edge(node, neighbor)

    for node, data in nodes.items():
        if not data["deleted"]:
            reduced.add_node(node)

    return original, reduced


def draw_graphs(original, reduced):
    # Both figures need to have the same layout
    pos = nx.spring_layout(original, seed=42)

    fig, axes = plt.subplots(1, 2, figsize=(12, 6))

    nx.draw(
        original,
        pos=pos,
        ax=axes[0],
        with_labels=True,
        node_color='lightblue',
        edge_color='gray',
        node_size=1200,
        font_size=9
    )
    axes[0].set_title("Original Graph")

    nx.draw(
        reduced,
        pos=pos,
        ax=axes[1],
        with_labels=True,
        node_color='lightcoral',
        edge_color='gray',
        node_size=1200,
        font_size=9
    )
    axes[1].set_title("Reduced Graph")

    plt.tight_layout()
    plt.show()


if __name__ == "__main__":
    file_path = "log.csv"

    original, reduced = load_graphs_from_csv(file_path)
    draw_graphs(original, reduced)