package org.ioopm.calculator;

import org.ioopm.calculator.ast.Clear;
import org.ioopm.calculator.ast.Environment;
import org.ioopm.calculator.ast.IllegalAssignmentException;
import org.ioopm.calculator.ast.IllegalExpressionException;
import org.ioopm.calculator.ast.Quit;
import org.ioopm.calculator.ast.SymbolicExpression;
import org.ioopm.calculator.ast.Variable;
import org.ioopm.calculator.ast.Vars;
import org.ioopm.calculator.parser.CalculatorParser;

public class Calculator {
 
    public static void main (String [] args){
        final CalculatorParser calculatorParser = new CalculatorParser();
        final Environment environment = new Environment();
        final Environment environmentCopy = new Environment();
        SymbolicExpression ans = null;

        int successfulReductions = 0;
        int nonSuccessfulReductions = 0;
        int expressions = 0;

        System.out.println("Welcome to the parser!");

        /**
         * A loop which simulates a calculator
         */
        while(true){
            environmentCopy.putAll(environment);
            SymbolicExpression Tree = null;
             try {
                System.out.print("Please enter an expression: ");
                String expression = System.console().readLine();
                expressions++;
                expression = expression.toLowerCase();
                Tree = calculatorParser.parse(expression, environment);
             }
             catch (IllegalAssignmentException iae) {
                System.out.println(iae.getMessage());
                environment.putAll(environmentCopy);
                continue;
             }
             catch (IllegalExpressionException iee) {
                System.out.println(iee.getMessage());
                continue;
             }
             catch(Exception e){
                System.out.println("Error3...");
             }

             if(Tree.isCommand()){
                if(Tree == Quit.instance()){
                    System.out.println("You have chosen to end the program");
                    break;
                }
                else if(Tree == Clear.instance()){
                    environment.clear();
                    System.out.println("You have now cleared the variables");
                }
                else if(Tree == Vars.instance()){
                    environment.forEach((key, value) -> System.out.println(key + " = " + value));
                }
            }

            else {
                try {
                    ans = Tree.eval(environment);
                    if(ans.isConstant())
                        successfulReductions++;
                    else
                        nonSuccessfulReductions++;

                    System.out.println(ans.toString()); 
                    environment.put(new Variable("ans"), ans);
                }
                catch (Exception e){
                    System.out.println("..");
                }
            }
        }       
        System.out.println("Amount of expressions: " + expressions);
        System.out.println("Successful reductions: " + successfulReductions);
        System.out.println("Non-Successful reductions: " + nonSuccessfulReductions);
    }
}
