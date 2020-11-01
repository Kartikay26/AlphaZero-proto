#include "common.h"

set<string> visited;

map<pair<string, int>, int> N;    // N(s, a) -- visit count
map<pair<string, int>, double> P;  // P(s, a) -- prior probabilities
map<pair<string, int>, double> Q;  // Q(s, a) -- action value function

// ========================= mcts core ==============================

// array<double, MAX_ACTIONS> mcts(GameState g, NeuralNet &nnet)
// {
//     // FAKE MCTS -- RETURNS UNIFORM DISTRIBUTION
//     return uniform(g, nnet);
// }

array<double, MAX_ACTIONS> mcts(GameState root, NeuralNet& nnet) {
    visited.clear();
    N.clear();
    P.clear();
    Q.clear();
    // perform NUM_SIMULATIONS simulations to leaf nodes...
    for (int i = 0; i < NUM_SIMULATIONS; i++) {
        simulate(root, nnet, true);
    }

    // ...and then report probs proportional to visit counts at root.
    array<double, MAX_ACTIONS> probs;
    for (int a = 0; a < MAX_ACTIONS; a++) {
        probs[a] = N[{root.hash(), a}] / double(NUM_SIMULATIONS - 1);
    }
    return probs;
}

// ====================== mcts simulate algo ========================

double simulate(GameState s,
               NeuralNet& nnet,
               int depth)  // -> returns evaluation of state s acc to s's player
{
    if (s.terminated()) {
        Outcome actual_outcome = s.evaluate();
        if (actual_outcome == Outcome(s.turn())) {
            return +1;
        } else if (actual_outcome == Outcome(opponent(s.turn()))) {
            return -1;
        } else  // draw
        {
            return 0;
        }
    }

    if (not visited.count(s.hash()))  // a new node has been reached
    {
        // add to visited
        visited.insert(s.hash());
        // evaluate the new node and store the probs etc
        Output output = nnet.predict(Image(s));
        array<double, MAX_ACTIONS> cur_probs = output.policy;
        if (depth == 0) {
            cur_probs = uniform(s, nnet);
        }
        for (int a = 0; a < MAX_ACTIONS; a++) {
            P[{s.hash(), a}] = cur_probs[a];
        }
        // return the output of the evaluation
        return output.evaluation;
    }

    // MAIN RECURSIVE PART (in - tree)

    // select best action according to UCB formula
    // u(s,a) = Q(s,a) + c_puct * sqrt(sum_b(N(s,b))) / (1 + N(s,a))

    auto possible = s.getPossibleActions();

    int best_action = -1;
    double best_u = -1e9;

    int total_branches_down = 0;
    for (int b = 0; b < MAX_ACTIONS; b++)
        total_branches_down += N[{s.hash(), b}];

    for (int a = 0; a < MAX_ACTIONS; a++) {
        if (not possible[a])
            continue;

        int cur_n = N[{s.hash(), a}];

        double cur_q = Q[{s.hash(), a}];
        double cur_p = P[{s.hash(), a}];

        double cur_u =
            cur_q + C_PUCT * cur_p * sqrt(total_branches_down) / (1 + cur_n);

        if (cur_u > best_u) {
            best_action = a;
            best_u = cur_u;
        }
    }

    // simulate down from new_s produced by best_action
    GameState new_s = s.playAction(best_action);
    double value = -simulate(
        new_s, nnet, depth + 1);  // negative sign because we hate our opponent

    // update N and Q value for (s,a)

    int old_n = N[{s.hash(), best_action}];
    N[{s.hash(), best_action}] = old_n + 1;

    double old_q = Q[{s.hash(), best_action}];
    Q[{s.hash(), best_action}] = old_q + (value - old_q) / (old_n + 1);

    // return value of leaf node (according to us)
    return value;
}

array<double, MAX_ACTIONS> uniform(GameState g, NeuralNet& nnet) {
    auto actions = g.getPossibleActions();
    int num_actions = accumulate(actions.begin(), actions.end(), 0);
    array<double, MAX_ACTIONS> probs;
    for (int i = 0; i < MAX_ACTIONS; i++) {
        if (actions[i]) {
            probs[i] = 1. / num_actions;
        } else {
            probs[i] = 0;
        }
    }
    return probs;
}

// ==================================================================