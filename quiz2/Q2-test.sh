#!/bin/bash

# Function to send a POST request to the server
send_post_request() {
    local command="$1"
    local data="$2"
    local url="localhost:8080"  # Replace with the actual server URL

    # Use curl to send the POST request
    curl -X POST -d "${command} ${data}" "${url}"
}

# Test cases
send_post_request "grade" "123"
send_post_request "print" "123"
send_post_request "grade" "456"
send_post_request "print" "456"
send_post_request "print" "ALL"

# Example: Print the server's reply after all the test cases
send_post_request "dummy"  # Replace "dummy" with any command that triggers a reply
