package org.ioopm.calculator.ast;

public class Exp extends Unary{

    /**
     * Creates an exponent object
     * @param expression
     */
    public Exp(SymbolicExpression expression){
        super(expression);
    }

    public String getName(){
        return "exp";
    }

    public int getPriority(){
        return 0;
    }

    public SymbolicExpression eval(Environment vars) {
        SymbolicExpression arg = this.argument.eval(vars);
        if (arg.isConstant()) {
            return new Constant(Math.exp(arg.getValue()));
        } else {
            return new Exp(arg.eval(vars));
        }
    }

     /**
     * Returns true is the objects are equal and false otherwise
     * @param other - The object to compare to
     */
    public boolean equals(Object other) {
        if (other instanceof Exp) {
            return this.equals((Exp) other);
        } else {
            return false;
        }
    }
    
    private boolean equals(Exp other) {
        return this.getArgument().equals(other.getArgument());
    }
}
