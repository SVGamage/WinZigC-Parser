#!/bin/bash

# Script to run all winzig test files and compare with expected output

echo "WinZigC Parser Test Runner"
echo "=========================="
echo

cd /home/vihanga-ms/Documents/Repos/WinZigC-Parser

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

total_tests=0
passed_tests=0
failed_tests=0

# Loop through all winzig test files
for i in {01..15}; do
    test_file="winzig_test_programs/winzig_$i"
    expected_file="winzig_test_programs/winzig_$i.tree"
    output_file="temp_output_$i.tree"
    
    if [ -f "$test_file" ] && [ -f "$expected_file" ]; then
        echo "Testing winzig_$i..."
        total_tests=$((total_tests + 1))
        
        # Run the parser on the test file with 10-second timeout
        timeout 10s ./winzigc -ast "$test_file" > "$output_file" 2>&1
        exit_code=$?
        
        # Check if the command timed out or had other errors
        if [ $exit_code -eq 124 ]; then
            echo -e "${RED}✗ TIMEOUT${NC}: winzig_$i exceeded 10-second limit (possible infinite loop)"
            failed_tests=$((failed_tests + 1))
        elif [ $exit_code -ne 0 ]; then
            echo -e "${RED}✗ ERROR${NC}: winzig_$i failed with exit code $exit_code"
            failed_tests=$((failed_tests + 1))
            echo "  Check temp_output_$i.tree for error details"
        else
            # Compare with expected output
            if diff -q "$output_file" "$expected_file" > /dev/null; then
                echo -e "${GREEN}✓ PASS${NC}: winzig_$i matches expected output"
                passed_tests=$((passed_tests + 1))
            else
                echo -e "${RED}✗ FAIL${NC}: winzig_$i does not match expected output"
                failed_tests=$((failed_tests + 1))
                echo "  Differences found. Use 'diff temp_output_$i.tree winzig_test_programs/winzig_$i.tree' to see details"
            fi
        fi
        
        # Clean up temporary file
        # rm -f "$output_file"  # Keep files for analysis
        echo
    fi
done

echo "=========================="
echo "Test Summary:"
echo -e "Total tests: $total_tests"
echo -e "${GREEN}Passed: $passed_tests${NC}"
echo -e "${RED}Failed: $failed_tests${NC}"

if [ $failed_tests -eq 0 ]; then
    echo -e "${GREEN}All tests passed!${NC}"
    exit 0
else
    echo -e "${RED}Some tests failed.${NC}"
    exit 1
fi
