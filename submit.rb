
files = Dir.glob("*.c")
files << Dir.glob("*.h")

files = files.filter { |f| !f.include?("test_") }

puts files.join(", ")
system("rm -rf submit")
system("mkdir submit")
system("cp #{files.join(" ")} submit")

main_content = File.read("src/main.c")
main_content = main_content.gsub(/#include ".*"/, "")

File.open("submit/domino.c", "a") do |f|
  f.write(main_content)
end

system("cd submit && gcc -O2 -std=c99 --pedantic *.c -o iap")
# puts "Running iap --challenge with params: 4 3 4 1 3 4 5  2 2"

# puts "Output:"
# a = system(%(cd submit && echo "4 3 4 1 3 4 5 2 2" | ./iap --challenge))

# system("rm -rf submit/iap")

# system("cp Relazione.* submit")

# system("cp Doxyfile submit")

# system("cd submit && doxygen Doxyfile")

# system("cd submit && zip -r kirpachov.zip *")
