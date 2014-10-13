/*************************************************************************
 *  Compilation:  javac DirectedEulerianCycle.java
 *  Dependencies: Digraph.java Stack.java Queue.java StdOut.java
 *
 *  Find an Euler tour if one exists.
 *
 *  Runs in O(E + V) time.
 *
 *
 *************************************************************************/

public class DirectedEulerianCycle {
    private Stack<Integer> tour = new Stack<Integer>();
    private boolean isEulerian = true;
    private Stack<Integer> stack = new Stack<Integer>();
    private Queue<Integer>[] adj;

    public DirectedEulerianCycle(Digraph G) {

        // create local copy of adjacency lists
        adj = (Queue<Integer>[]) new Queue[G.V()];
        for (int v = 0; v < G.V(); v++) {
            adj[v] = new Queue<Integer>();
            for (int w : G.adj(v))
                adj[v].enqueue(w);
        }

        // find Eulerian tour
        stack.push(0);
        while (!stack.isEmpty()) {
            int s = stack.pop();
            tour.push(s);
            int v = s;
            while (!adj[v].isEmpty()) {
                stack.push(v);
                v = adj[v].dequeue();
            }
            if (v != s) isEulerian = false;
        }

        // check if all edges have been used
        for (int v = 0; v < G.V(); v++)
            if (!adj[v].isEmpty()) isEulerian = false;
    }

    // return Eulerian tour, or null if no such tour
    public Iterable<Integer> tour() {
        if (!isEulerian) return null;
        return tour;
    }

    // does the digraph have an Eulerian tour?
    public boolean isEulerian() {
        return isEulerian;
    }
}
