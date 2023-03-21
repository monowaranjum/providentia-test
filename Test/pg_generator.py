import graphviz
import os

def view_the_prov_graph(file_name, param):
    pg = graphviz.Digraph('PG', filename = 'prov_g'+str(param)+'.gv')
    pg.node("process_name", label="Artifact:Process\nprocess_name")
    pg_created_set = set()
    pg_created_set.add("process_name")
    with open(file_name, "r") as prov_file:
        for line in prov_file:
            data = line.split(",")
            if data[2] not in pg_created_set:
                pg.node(data[2], label="Artifact:"+data[1]+":"+data[2])
                pg_created_set.add(data[2])
            pg.edge("process_name",data[2],label=data[0])

    pg.attr(label="\n\nProvenance Graph\n Of Current Program")
    pg.attr(fontsize='20')

    pg.view()
    


provenance_files = os.listdir()
count = 0
for file in provenance_files:
    full_file_name = os.path.join(os.path.abspath(os.getcwd()), file)
    view_the_prov_graph(full_file_name, count)
    count +=1
