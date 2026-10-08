package org.ioopm.calculator.ast;

public class Negation extends Unary{
    public Negation(SymbolicExpression expression){
        super(expression);
    }

    public String getName(){
        return "-";
    }

    public SymbolicExpression eval(Environment vars){
        this.argument = this.argument.eval(vars);
        if(this.argument.isConstant())
            return new Constant(-this.getArgument().getValue());
        else 
            return this;
    }

    /**
     * Returns true is the objects are equal and false otherwise
     * @param other - The object to compare to
     */
    public boolean equals(Object other) {
        if (other instanceof Negation) {
            return this.equals((Negation) other);
        } else {
            return false;
        }
    }

    private boolean equals(Negation other) {
        return this.getArgument().equals(other.getArgument());
    }
}
