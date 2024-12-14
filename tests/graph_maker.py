# TODO: Fix pylance/pylint errors. Script is working as intended though.
# type: ignore
# pylint:skip-file

import matplotlib.pyplot as plt
import networkx as nx
import os
import sys

# Function to create and save the graph
def create_graph(test_name):
    # Build the path dynamically based on the current directory
    current_dir = os.getcwd()
    input_file = os.path.join(current_dir, f"{test_name}.in")
    output_file = os.path.join(current_dir, f"{test_name}.png")
    
    # Open and parse the input file
    with open(input_file, 'r') as f:
        lines = f.readlines()

    # Parse the first line: n = nodes, m = edges, l = lines
    n, m, l = map(int, lines[0].split())
    edges = [tuple(map(int, line.split())) for line in lines[1:]]

    # Create the graph
    G = nx.MultiGraph()
    for x, y, line in edges:
        G.add_edge(x, y, line=line)

    # Generate positions for nodes
    pos = nx.kamada_kawai_layout(G)

    # Define a list of bright, visually distinct colors
    bright_colors = [
        "#FF5733",  # Red-Orange
        "#33C1FF",  # Sky Blue
        "#75FF33",  # Bright Green
        "#FFC300",  # Yellow
        "#DA33FF",  # Bright Purple
        "#FF33A6",  # Pink
        "#33FFBD",  # Teal
        "#FF6F61",  # Light Red
        "#B833FF",  # Dark Pink
        "#33A1FF"   # Light Blue
    ]

    # Dynamically assign colors to unique line types
    unique_lines = sorted(set(line for _, _, line in edges))
    line_colors = {line: bright_colors[i % len(bright_colors)] for i, line in enumerate(unique_lines)}

    # Draw the graph
    plt.figure(figsize=(8, 6))
    nx.draw_networkx_nodes(G, pos, node_color='white', edgecolors='black', node_size=700, linewidths=1.5)
    nx.draw_networkx_labels(G, pos, font_size=12, font_weight='bold')

    # Draw edges and assign colors based on their 'line' attribute
    for x, y, line in edges:
        nx.draw_networkx_edges(G, pos, edgelist=[(x, y)], width=4, edge_color=line_colors[line], style='solid')
    
    # Create a dynamic legend for line colors
    from matplotlib.lines import Line2D
    legend_elements = [
        Line2D([0], [0], color=color, lw=4, label=f"Line {line}") for line, color in line_colors.items()
    ]

    # Dynamically position the legend to avoid overlap
    plt.legend(handles=legend_elements, loc='best', fontsize='10', frameon=True)

    # Save the graph to a file
    plt.axis('off')
    plt.tight_layout()
    plt.savefig(output_file)
    plt.close()  # Close the figure without displaying it

# Main execution
if __name__ == "__main__":
    # Check if a test name is passed as an argument
    if len(sys.argv) != 2:
        print("Usage: python3 graph_maker.py <test_name>")
    else:
        test_name = sys.argv[1]
        create_graph(test_name)
