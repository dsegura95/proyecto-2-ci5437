#include "../othello_cut.h" // won't work correctly until .h is fixed!
#include <functional>

int infinity = INT32_MAX;

int negamax(othello_t state, bool color){
    int color_num = (color) ? 1 : -1;
    if (state.terminal()) return color_num * state.value();
    int alpha = infinity * -1;

    vector<int> childs = state.get_valid_moves(color);
    for (int i = 0; i < childs.size(); i++){
        alpha = max(alpha, -negamax(state.move(color, childs[i]), !color));
    }
    return alpha;
}

int negamax_ab(othello_t state, unsigned depth, int alpha, int beta, bool color){
    int color_num = (color) ? 1 : -1;
    if (depth == 0 || state.terminal()) return color_num * state.value();
    int score = infinity * -1;

    vector<int> childs = state.get_valid_moves(color);
    for (int i = 0; i < childs.size(); i++){
        int val = -negamax_ab(state.move(color, childs[i]), depth - 1, beta*-1, alpha*-1, !color);
        score = max(score, val);
        alpha = max(alpha, val);
        if (alpha >= beta) break;
    }
    return score;
}

int scout(othello_t state, unsigned depth, bool color){
    if (depth == 0 || state.terminal()) return state.value();
    int score = infinity * -1;

    vector<int> childs = state.get_valid_moves(color);
    for (int i = 0; i < childs.size(); i++){
        if (i == 0){
            score = scout(state.move(color, childs[i]), depth-1, !color);
        } else {
            if (color && TEST(state.move(color, childs[i]), depth-1, score, std::greater<int>(), !color)){
                score = scout(state.move(color, childs[i]), depth-1, !color);
            }
            else if(!color && !TEST(state.move(color, childs[i]), depth-1, score, std::greater_equal<int>(), !color)){
                score = scout(state.move(color, childs[i]), depth-1, !color);
            }
        }
    }
    return score;
}

bool TEST(othello_t state, unsigned depth, int score, std::function<bool (int, int)> compare, bool color){
    if (depth == 0 || state.terminal()) return compare(state.value(), score);

    vector<int> childs = state.get_valid_moves(color);
    for (int i = 0; i < childs.size(); i++){
        if (i == 0){
            if (color && TEST(state.move(color, childs[i]), depth-1, score, std::greater<int>(), !color)){
                return true;
            }
            else if(!color && !TEST(state.move(color, childs[i]), depth-1, score, std::greater_equal<int>(), !color)){
                return false;
            }
        }
    }
    return (!color);
}

int negascout(othello_t state, unsigned depth, int alpha, int beta, bool color){
    int color_num = (color) ? 1 : -1;
    if (depth == 0 || state.terminal()) return color_num * state.value();
    int score;

    vector<int> childs = state.get_valid_moves(color);
    for (int i = 0; i < childs.size(); i++){
        if (i == 0){
            score = -negascout(state.move(color, childs[i]), depth - 1, beta*-1, alpha*-1, !color);
        } else {
            score = -negascout(state.move(color, childs[i]), depth - 1, (alpha*-1) -1, alpha*-1, !color);
            if (alpha < score &&  score < beta) score = -negascout(state.move(color, childs[i]), depth - 1, beta*-1, score*-1, !color);
        }
        alpha = max(alpha, score);
        if (alpha >= beta) break;
    }
    return alpha;
}