# type: ignore
# pylint:skip-file
import matplotlib.pyplot as plt
import networkx as nx
import os
import sys
from matplotlib.lines import Line2D

# Function to create and save the graph
def create_graph(test_name):
    # Build the path dynamically based on the current directory
    current_dir = os.getcwd()
    input_file = os.path.join(current_dir, f"{test_name}.in")
    output_file = os.path.join(current_dir, f"{test_name}.png")  # Use test_name.png as the output file
    
    # Open and parse the input file
    with open(input_file, 'r') as f:
        lines = f.readlines()

    # Parse the first line: n = nodes, m = edges, l = lines
    n, m, l = map(int, lines[0].split())
    edges = [tuple(map(int, line.split())) for line in lines[1:]]

    # Create the graph
    G = nx.MultiGraph()
    G.add_nodes_from(range(1, n + 1))  # Add all nodes to the graph
    for x, y, line in edges:
        G.add_edge(x, y, line=line)

    # Dynamically generate a list of bright, visually distinct colors based on the number of unique lines
    unique_lines = sorted(set(line for _, _, line in edges))
    bright_colors = plt.cm.tab10.colors  # Using the tab10 color map which has 10 distinct colors

    # Ensure that each unique line gets a color, and use modulo to repeat colors if there are more lines than colors
    line_colors = {line: bright_colors[i % len(bright_colors)] for i, line in enumerate(unique_lines)}

    # Node placement optimized to reduce crossings
    pos = nx.kamada_kawai_layout(G)

    # Function to generate a curved path
    def get_curve(xy1, xy2, concave_up=True, curvature=0.1):
        x1, y1 = xy1
        x2, y2 = xy2
        if concave_up:
            control = [(x1 + x2) / 2, (y1 + y2) / 2 + curvature]
        else:
            control = [(x1 + x2) / 2, (y1 + y2) / 2 - curvature]
        return [xy1, control, xy2]

    # Draw the graph
    plt.figure(figsize=(8, 6))
    nx.draw_networkx_nodes(G, pos, node_color='white', edgecolors='black', node_size=1000)
    nx.draw_networkx_labels(G, pos, font_size=12, font_weight='bold')

    # Track edges drawn to handle overlapping
    edge_drawn = {}  # Store edge instances to alternate between curvatures
    for (u, v, line) in edges:
        edge_key = (min(u, v), max(u, v))
        if edge_key in edge_drawn:
            curvature = 0.3 * (-1)**edge_drawn[edge_key]  # Alternate curvature for multiple edges
            edge_drawn[edge_key] += 1
        else:
            curvature = 0.0  # Straight edge for the first instance
            edge_drawn[edge_key] = 1

        # Draw the edge with alternating curvatures
        nx.draw_networkx_edges(
            G, pos, edgelist=[(u, v)],
            width=5, alpha=0.8, edge_color=line_colors[line],
            connectionstyle=f"arc3,rad={curvature}"
        )

    # Create a dynamic legend for line colors
    legend_elements = [
        Line2D([0], [0], color=color, lw=4, label=f'Line {line}')
        for line, color in line_colors.items()
    ]
    
    # Use bbox_to_anchor to place the legend outside the graph to the right
    plt.legend(handles=legend_elements, loc='upper left', bbox_to_anchor=(1, 1), fontsize=10, frameon=False)

    # Remove the axis and the surrounding border
    plt.axis('off')

    # Save the graph with a white background and no border
    plt.savefig(output_file, bbox_inches='tight', transparent=False, facecolor='white')  # White background
    plt.close()  # Close the figure without displaying it

# Main execution
if __name__ == "__main__":
    # Check if a test name is passed as an argument
    if len(sys.argv) != 2:
        print("Usage: python3 graph_maker.py <test_name>")
    else:
        test_name = sys.argv[1]
        create_graph(test_name)
