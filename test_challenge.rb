# Testing challenge #1

unless File.exists?("submit/iap")
  puts "Executable should be in submit/iap"
  puts "Executable not found!"
  exit 1
end

def test_output(output, sum:, line:)
  output_sum = output.split(" ").map(&:to_i).filter(&:positive?).sum
  success = true

  if output_sum != sum
    puts "Line #{line} | SUM NOT MATCHING. output: \"#{output.gsub("\n", '')}\" | sum: #{sum} | output_sum: #{output_sum}"
    success = false
  end

  success
end

test_output(`echo "4  3 4  1 3  4 5  2 2" | submit/iap --challenge`, sum: 20, line: __LINE__)
test_output(`echo "4  3 4  3 1  5 4  2 2" | submit/iap --challenge`, sum: 20, line: __LINE__)
test_output(`echo "4  4 3  3 1  5 4  2 2" | submit/iap --challenge`, sum: 20, line: __LINE__)
test_output(`echo "4  4 3  1 3  5 4  2 2" | submit/iap --challenge`, sum: 20, line: __LINE__)
test_output(`echo "4  4 3  1 3  4 5  2 2" | submit/iap --challenge`, sum: 20, line: __LINE__)
test_output(`echo "4  4 3  3 1  4 5  2 2" | submit/iap --challenge`, sum: 20, line: __LINE__)
test_output(`echo "4  3 4  3 1  4 5  2 2" | submit/iap --challenge`, sum: 20, line: __LINE__)

test_output(`echo "4  1 2  2 3  3 4  4 4" | submit/iap --challenge`, sum: 1+2+2+3+3+4+4+4, line: __LINE__)
test_output(`echo "4  2 1  2 3  3 4  4 4" | submit/iap --challenge`, sum: 1+2+2+3+3+4+4+4, line: __LINE__)
test_output(`echo "4  2 1  3 2  3 4  4 4" | submit/iap --challenge`, sum: 1+2+2+3+3+4+4+4, line: __LINE__)
test_output(`echo "4  2 1  3 2  4 3  4 4" | submit/iap --challenge`, sum: 1+2+2+3+3+4+4+4, line: __LINE__)
test_output(`echo "4  2 1  3 2  4 4  4 3" | submit/iap --challenge`, sum: 1+2+2+3+3+4+4+4, line: __LINE__)
test_output(`echo "4  2 1  4 4  3 2  4 3" | submit/iap --challenge`, sum: 1+2+2+3+3+4+4+4, line: __LINE__)
test_output(`echo "4  4 4  2 1  3 2  4 3" | submit/iap --challenge`, sum: 1+2+2+3+3+4+4+4, line: __LINE__)

test_output(`echo "10  6 6  6 6  6 6  3 6  1 6  2 6  1 4  3 4  4 4  4 5" | submit/iap --challenge`, sum: 4+4+4+1+1+6+6+6+6+6+6+6+6+3+3+4+4+5, line: __LINE__)
test_output(`echo "10  6 6  6 6  6 6  3 6  1 6  2 6  1 4  3 4  4 5  4 4" | submit/iap --challenge`, sum: 4+4+4+1+1+6+6+6+6+6+6+6+6+3+3+4+4+5, line: __LINE__)
test_output(`echo "10  6 6  6 6  6 6  3 6  1 6  2 6  1 4  4 5  3 4  4 4" | submit/iap --challenge`, sum: 4+4+4+1+1+6+6+6+6+6+6+6+6+3+3+4+4+5, line: __LINE__)

# WITH SPECIALS
test_output(`echo "3  6 6  0 0  5 5" | submit/iap --challenge`, sum: 5 + 5 + 6 + 6, line: __LINE__)
test_output(`echo "3  0 0  6 6  5 5" | submit/iap --challenge`, sum: 5 + 5 + 6 + 6, line: __LINE__)
test_output(`echo "3  11 11  6 6  5 5" | submit/iap --challenge`, sum: 6 * 2 * 3, line: __LINE__)
test_output(`echo "3  11 11  6 6  3 3" | submit/iap --challenge`, sum: 4 * 2 * 2, line: __LINE__)
test_output(`echo "3  6 6  11 11  3 3" | submit/iap --challenge`, sum: 4 * 2 * 2, line: __LINE__)
test_output(`echo "3  1 2  12 21  1 1" | submit/iap --challenge`, sum: 8, line: __LINE__)
test_output(`echo "3  5 6  12 21  11 11" | submit/iap --challenge`, sum: 5 + 6 + 6 + 5, line: __LINE__)

