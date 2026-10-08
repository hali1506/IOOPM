package org.ioopm.calculator.ast;

public class Unary extends SymbolicExpression {

    SymbolicExpression argument;

    /**
     * Creates a unary object
     * 
     * @param argument - The argument of the expression
     */
    public Unary(SymbolicExpression argument) {
        super();
        this.argument = argument;
    }

    /**
     * Gets the argument of the expression
     * 
     * @return
     */
    public SymbolicExpression getArgument() {
        return this.argument;
    }

    /**
     * Returns the expression in string format
     */
    public String toString() {
        return this.getName() + "" + this.argument.toString();
    }
}
