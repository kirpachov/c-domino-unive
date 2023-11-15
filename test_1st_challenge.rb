# Testing challenge #1

unless File.exists?("submit/iap")
  puts "Executable should be in submit/iap"
  puts "Executable not found!"
  exit 1
end

def test_output(output, sum:)
  output_sum = output.split(" ").map(&:to_i).filter(&:positive?).sum
  success = true

  if output_sum != sum
    puts "SUM NOT MATCHING. output: #{output} | sum: #{sum} | output_sum: #{output_sum}"
    success = false
  end

  success
end

test_output(`echo "4 3 4 1 3 4 5 2 2" | submit/iap --challenge`, sum: 20)
test_output(`echo "4 1 2 2 3 3 4 4 4" | submit/iap --challenge`, sum: 1+2+2+3+3+4+4+4)

# [
#   { dominoes: [[3, 4], [1, 3], [4, 5], [2, 2]], expected: 20 }
# ].each do |scenario|
#   a = system(%(echo "#{scenario[:dominoes].count} #{scenario[:dominoes].join(" ")}" | submit/iap --challenge))
#
#   unless a
#     puts "FAILED"
#     exit 1
#   end
# end
