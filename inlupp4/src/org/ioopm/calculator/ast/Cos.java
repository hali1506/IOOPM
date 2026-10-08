package org.ioopm.calculator.ast;

public class Cos extends Unary {
    /**
     * Creates a cos object
     * 
     * @param expression - the expression to calculate the cos value of
     */
    public Cos(SymbolicExpression expression) {
        super(expression);
    }

    /**
     * Returns the name of the operation
     */
    public String getName() {
        return "cos";
    }

    /**
     * Evaluates the expression
     */
    public SymbolicExpression eval(Environment vars) {
        SymbolicExpression arg = this.argument.eval(vars);
        if (arg.isConstant()) {
            return new Constant(Math.cos(arg.getValue()));
        } else {
            return new Cos(arg.eval(vars));
        }
    }

    /**
     * Returns true is the objects are equal and false otherwise
     * 
     * @param other - The object to compare to
     */
    public boolean equals(Object other) {
        if (other instanceof Cos) {
            return this.equals((Cos) other);
        } else {
            return false;
        }
    }

    private boolean equals(Cos other) {
        return this.getArgument().equals(other.getArgument());
    }

    @Override
    public String toString() {
        return getName() + "(" + this.getArgument() + ")";
    }
}
