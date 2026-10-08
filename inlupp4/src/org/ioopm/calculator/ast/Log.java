package org.ioopm.calculator.ast;

public class Log extends Unary {
    /**
     * Creates a log object
     * @param expression - the expression to calculate the log value of
     */
    public Log(SymbolicExpression expression){
        super(expression);
        priority = 0;
    }

    /**
     * Returns the name of the operation
     */
    public String getName()
    {
        return "log";
    } 

    /**
     * Evaluates the expression
     */
    public SymbolicExpression eval(Environment vars) {
        SymbolicExpression arg = this.argument.eval(vars);
        if (arg.isConstant()) {
            return new Constant(Math.log(arg.getValue()));
        } else {
            return new Log(arg.eval(vars));
        }
    }

     /**
     * Returns true is the objects are equal and false otherwise
     * @param other - The object to compare to
     */
    public boolean equals(Object other) {
        if (other instanceof Log) {
            return this.equals((Log) other);
        } else {
            return false;
        }
    }
    
    private boolean equals(Log other) {
        return this.getArgument().equals(other.getArgument());
    }
}
