#include "common.h"

set<string> visited;

map<pair<string, int>, int> N;   // N(s, a) -- visit count
map<pair<string, int>, float> P; // P(s, a) -- prior probabilities
map<pair<string, int>, float> Q; // Q(s, a) -- action value function

// ========================= mcts core ==============================

array<float, MAX_ACTIONS> mcts(GameState root, NeuralNet &nnet)
{
    // perform NUM_SIMULATIONS simulations to leaf nodes...
    for (int i = 0; i < NUM_SIMULATIONS; i++)
    {
        simulate(root, nnet);
    }

    // ...and then report probs proportional to visit counts at root.
    array<float, MAX_ACTIONS> probs;
    for (int a = 0; a < MAX_ACTIONS; a++)
    {
        probs[a] = N[{root.hash(), a}] / float(NUM_SIMULATIONS);
    }
    return probs;
}

// ====================== mcts simulate algo ========================

float simulate(GameState s, NeuralNet &nnet) // -> returns evaluation of state s acc to s's player
{
    if (s.terminated())
    {
        Outcome actual_outcome = s.evaluate();
        if (actual_outcome == Outcome(s.turn()))
        {
            return +1;
        }
        else if (actual_outcome == Outcome(opponent(s.turn())))
        {
            return -1;
        }
        else // draw
        {
            return 0;
        }
    }

    if (not visited.count(s.hash())) // a new node has been reached
    {
        // add to visited
        visited.insert(s.hash());
        // evaluate the new node and store the probs etc
        Output output = nnet.predict(Image(s));
        for (int a = 0; a < MAX_ACTIONS; a++)
        {
            P[{s.hash(), a}] = output.policy[a];
        }
        // return the output of the evaluation
        return output.evaluation;
    }

    // MAIN RECURSIVE PART (in - tree)

    // select best action according to UCB formula
    // u(s,a) = Q(s,a) + c_puct * sqrt(sum_b(N(s,b))) / (1 + N(s,a))

    auto possible = s.getPossibleActions();

    int best_action = -1;
    float best_u = -1e9;

    int total_branches_down = 0;
    for (int b = 0; b < MAX_ACTIONS; b++)
        total_branches_down += N[{s.hash(), b}];

    for (int a = 0; a < MAX_ACTIONS; a++)
    {
        if (not possible[a])
            continue;

        int cur_n = N[{s.hash(), a}];

        float cur_q = Q[{s.hash(), a}];
        float cur_p = P[{s.hash(), a}];

        float cur_u = cur_q + C_PUCT * cur_p * sqrt(total_branches_down) / (1 + cur_n);

        if (cur_u > best_u)
        {
            best_action = a;
            best_u = cur_u;
        }
    }

    // simulate down from new_s produced by best_action
    GameState new_s = s.playAction(best_action);
    float value = -simulate(new_s, nnet); // negative sign because we hate our opponent

    // update N and Q value for (s,a)

    int old_n = N[{s.hash(), best_action}];
    N[{s.hash(), best_action}] = old_n + 1;

    float old_q = Q[{s.hash(), best_action}];
    Q[{s.hash(), best_action}] = old_q + (value - old_q) / (old_n + 1);

    // return value of leaf node (according to us)
    return value;
}

// ==================================================================