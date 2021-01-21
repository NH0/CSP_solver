import networkx as nx

N = 100
#"gen_dorogovtsev" : nx.dorogovtsev_goltsev_mendes_graph,
#
#"gen_binomial" : nx.binomial_tree,
list_of_graphs = {"gen_complete" : nx.complete_graph,
                  "gen_circular_ladder" : nx.circular_ladder_graph,
                  "gen_cycle" : nx.cycle_graph,
                  "gen_ladder" : nx.ladder_graph,
                  "gen_path" : nx.path_graph,
                  "gen_star" : nx.star_graph,
                  "gen_wheel" : nx.wheel_graph,
                  "gen_random_tree" : nx.random_tree,
                  "gen_internet" : nx.random_internet_as_graph}

def graph_to_col(g, name):
    with open("./col/" + name + "_" + N + ".col", 'w') as f:
        f.write("c Automatically generated file\n")
        f.write("c FILE: " + name + ".col\n")
        f.write("p edge "+ str(g.number_of_nodes()) + " " + str(g.number_of_edges()) + "\n")
        for edge in g.edges:
            e1 = str(edge[0] + 1)
            e2 = str(edge[1] + 1)
            f.write("e " + e1 + " " + e2 + "\n")

def generate_col(name, n = N):
    g = list_of_graphs[name](n)
    graph_to_col(g, name)

for k,v in list_of_graphs.items():
    generate_col(k)

g1 = nx.fast_gnp_random_graph(N, 0.85)
graph_to_col(g1, "gen_erdos")

g2 = nx.newman_watts_strogatz_graph(N, 5, 0.6)
graph_to_col(g2, "gen_small_world")

g3 = nx.barabasi_albert_graph(N, 5)
graph_to_col(g3, "gen_barbasi")

g4 = nx.karate_club_graph()
graph_to_col(g4, "gen_karate_club")
