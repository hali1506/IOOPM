all:
	javac -d classes -sourcepath src src/org/ioopm/calculator/Calculator.java

run:
	java -cp classes org.ioopm.calculator.Calculator

clean:
	rm -rf classes

test:
#javac -d classes -sourcepath src src/org/ioopm/calculator/CalcTests.java
	javac -cp ./src/org/ioopm/calculator/junit-platform-console-standalone-1.8.1.jar src/org/ioopm/calculator/CalcTests.java

testrun:
#java -cp classes org.ioopm.calculator.CalcTests
	java -jar ./src/org/ioopm/calculator/junit-platform-console-standalone-1.8.1.jar -cp src/org/ioopm/calculator -c CalcTests
