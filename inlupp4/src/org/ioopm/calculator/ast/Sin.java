package org.ioopm.calculator.ast;

public class Sin extends Unary {
    /**
     * Creates a cos object
     * 
     * @param expression - the expression to calculate the cos value of
     */
    public Sin(SymbolicExpression arg) {
        super(arg);
        priority = 0;
    }

    /**
     * Gets the name of the operation
     */
    public String getName() {
        return "sin";
    }

    /**
     * Evaluates the expression
     */
    public SymbolicExpression eval(Environment vars) {
        SymbolicExpression arg = this.argument.eval(vars);
        if (arg.isConstant()) {
            return new Constant(Math.sin(arg.getValue()));
        } else {
            return new Sin(arg.eval(vars));
        }
    }

    /**
     * Returns true is the objects are equal and false otherwise
     * 
     * @param other - The object to compare to
     */
    public boolean equals(Object other) {
        if (other instanceof Sin) {
            return this.equals((Sin) other);
        } else {
            return false;
        }
    }

    private boolean equals(Sin other) {
        return this.getArgument().equals(other.getArgument());
    }

    @Override
    public String toString() {
        return getName() + "(" + this.getArgument() + ")";
    }
}
