package org.ioopm.calculator.ast;

public class Addition extends Binary 
{
    /**
     * Creates an addition object
     * @param lhs - The left hand side of the operation
     * @param rhs - The right hand side of the operation
     */
    public Addition(SymbolicExpression lhs, SymbolicExpression rhs)
    {
        super(lhs, rhs);
    }
    
    /**
     * Returns the name of the operation
     */   
    public String getName()
    {
        return "+";
    }

    /**
     * Returns the priority of the operation
     */  
    public int getPriority(){
        return 2;
    }
    
    /**
     * Returns true is the objects are equal and false otherwise
     * @param other - The object to compare to
     */
    public boolean equals(Object other) {
        if (other instanceof Addition ) {
            return this.equals((Addition) other);
        } else {
            return false;
        }
    }

    private boolean equals(Addition other) {
        return this.getRhs().equals(other.getRhs()) && this.getLhs().equals(other.getLhs());
    } 

    /**
     * Evaluates the expression
     * @param vars - A hash map of variables
     */
    public SymbolicExpression eval(Environment vars){
        this.setLhs(this.getLhs().eval(vars));
        this.setRhs(this.getRhs().eval(vars));
        if(this.getLhs().isConstant() && this.getRhs().isConstant()){
            return new Constant(this.getLhs().getValue() + this.getRhs().getValue());
        }
        else if(this.getLhs().isConstant()){
            return new Addition(this.getLhs(), this.getRhs().eval(vars));
        }
        else if(this.getRhs().isConstant()){
            return new Addition(this.getLhs().eval(vars), this.getRhs());
        }
        else {
            return new Addition(this.getLhs().eval(vars), this.getRhs().eval(vars));
        }
    }
}
