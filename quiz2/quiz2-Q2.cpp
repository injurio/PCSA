#include <iostream>
#include <unordered_map>
#include <string>
#include <sstream>
#include <cstdlib>
#include <cstdio>

// Function to process the POST command body
void processPostBody(const std::string& body, std::unordered_map<std::string, int>& studentScores) {
    std::istringstream stream(body);
    std::string command, data;

    while (stream >> command >> data) {
        if (command == "grade") {
            std::string command = "./dlc";
            std::string directory = "./" + data;
            std::string commandToRun = command + " " + directory;
            
            // Check if the script exists
            FILE* testScript = fopen(command.c_str(), "r");
            if (!testScript) {
                std::cerr << "Error: The script '" << command << "' is not found." << std::endl;
                return;
            }
            fclose(testScript);

            FILE* pipe = popen(commandToRun.c_str(), "r");
            if (!pipe) {
                std::cerr << "Error executing dlc command." << std::endl;
                return;
            }

            char buffer[128];
            std::string result = "";
            while (!feof(pipe)) {
                if (fgets(buffer, 128, pipe) != NULL)
                    result += buffer;
            }

            pclose(pipe);

            // Parse the score from the result
            size_t scoreStart = result.find("Score: ");
            if (scoreStart != std::string::npos) {
                size_t scoreEnd = result.find('/', scoreStart);
                if (scoreEnd != std::string::npos) {
                    std::string scoreStr = result.substr(scoreStart + 7, scoreEnd - scoreStart - 7);
                    int score = std::stoi(scoreStr);
                    // Store the score for the student
                    studentScores[data] = score;
                }
            }
        } else if (command == "print") {
            if (data == "ALL") {
                // Print all student scores
                for (const auto& entry : studentScores) {
                    std::cout << "StudentID: " << entry.first << ", Score: " << entry.second << std::endl;
                }
            } else {
                // Print score for a specific student
                auto it = studentScores.find(data);
                if (it != studentScores.end()) {
                    std::cout << "Score for StudentID " << data << ": " << it->second << std::endl;
                } else {
                    std::cout << "StudentID " << data << " not found or not graded." << std::endl;
                }
            }
        }
    }
}

// Function to construct the reply with key-value pairs
std::string constructReply(const std::unordered_map<std::string, int>& studentScores) {
    std::ostringstream reply;

    for (const auto& entry : studentScores) {
        reply << "StudentID: " << entry.first << ", Score: " << entry.second << "\n";
    }

    return reply.str();
}

int main() {
    std::unordered_map<std::string, int> studentScores;

    // Simulate receiving a POST request with a body
    std::string postBody = "grade 123\nprint 123\ngrade 456\nprint 456\nprint ALL\n";
    processPostBody(postBody, studentScores);

    // Construct and print the reply
    std::string reply = constructReply(studentScores);
    std::cout << "Server Reply:\n" << reply;

    return 0;
}
