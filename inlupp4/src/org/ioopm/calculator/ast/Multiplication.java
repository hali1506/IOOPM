package org.ioopm.calculator.ast;

public class Multiplication extends Binary 
{
    /**
     * Crates a multiplication object
     * @param lhs - the left hand side of the expression
     * @param rhs - the right hand side of the expression
     */
    public Multiplication(SymbolicExpression lhs, SymbolicExpression rhs){
        super(lhs, rhs);
        priority = 1;   
    }

    /**
     * Returns true since the expression is a multiplication object
     */
     public boolean isMultiplication(){
        return true;
    }

    /**
     * Returns the name of the operation
     */
    public String getName(){
        return "*";
    }

    /**
     * Returns the priority of the operation
     */
    public int getPriority(){
        return 1;
    }

    /**
     * Returns true if the objects are equal and false otherwise
     */
    public boolean equals(Object other) {
        if (other instanceof Multiplication ) {
            return this.equals((Multiplication) other);
        } else {
            return false;
        }
    }

    private boolean equals(Multiplication other) {
        return this.getRhs().equals(other.getRhs()) && this.getLhs().equals(other.getLhs());
    }

    /**
     * 
     */
    public SymbolicExpression eval(Environment vars){
        this.setLhs(this.getLhs().eval(vars));
        this.setRhs(this.getRhs().eval(vars));
        if(this.getLhs().isConstant() && this.getRhs().isConstant()){
            return new Constant(this.getLhs().getValue() * this.getRhs().getValue());
        }
        else if(this.getLhs().isConstant()){
            return new Multiplication(this.getLhs(), this.getRhs().eval(vars));
        }
        else if(this.getRhs().isConstant()){
            return new Multiplication(this.getLhs().eval(vars), this.getRhs());
        }
        else {
            return new Multiplication(this.getLhs().eval(vars), this.getRhs().eval(vars));
        }
    }
}
