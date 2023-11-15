#include <crow.h>
#include <iostream>
#include <map>

using namespace std;
using namespace crow;

map<string, int> scores;

int process_datalab_grading(const string& student_id) {
    // Replace this with the actual command you need to run
    // For simplicity, we'll just simulate a dummy score
    string dummy_score_script = "echo \"Score: 90/100\"";
    
    FILE* pipe = popen(dummy_score_script.c_str(), "r");
    if (!pipe) return -1;

    char buffer[128];
    string result = "";
    while (!feof(pipe)) {
        if (fgets(buffer, 128, pipe) != nullptr)
            result += buffer;
    }
    pclose(pipe);

    // Parse the score from the last line
    size_t pos = result.find("Score: ");
    if (pos != string::npos) {
        int score = stoi(result.substr(pos + 7, 2));
        return score;
    }

    return -1;
}

int main() {
    Crow<SimpleApp> app;

    // POST endpoint for processing commands
    CROW_ROUTE(app, "/")
        .methods("POST"_method)
        ([](const request& req) {
            auto content = req.body;
            stringstream ss(content);
            string line;

            while (getline(ss, line)) {
                if (line.find("grade") != string::npos) {
                    string _, student_id;
                    ss >> _ >> student_id;
                    int score = process_datalab_grading(student_id);
                    scores[student_id] = score;
                } else if (line.find("print") != string::npos) {
                    string _, target;
                    ss >> _ >> target;

                    if (target == "ALL") {
                        stringstream response_ss;
                        for (const auto& entry : scores) {
                            response_ss << entry.first << ": " << entry.second << "\n";
                        }
                        return response_ss.str();
                    } else {
                        return to_string(scores[target]);
                    }
                }
            }

            // Construct reply
            stringstream reply_ss;
            for (const auto& entry : scores) {
                reply_ss << entry.first << ": " << entry.second << "\n";
            }
            return reply_ss.str();
        });

    app.port(5000).multithreaded().run();
    return 0;
}
