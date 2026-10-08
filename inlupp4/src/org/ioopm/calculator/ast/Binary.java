package org.ioopm.calculator.ast;

public abstract class Binary extends SymbolicExpression{
    private SymbolicExpression lhs;
    private SymbolicExpression rhs; 

    /**
     * Creates a Binary object
     * @param lhs - The left hand side of the expression
     * @param rhs - The right hand side of the expression
     */
    public Binary(SymbolicExpression lhs, SymbolicExpression rhs){
        this.lhs = lhs;
        this.rhs = rhs;
    }

    /**
     * Always throws RuntimeException since we can not evaluate a binary
     */
    public SymbolicExpression eval(Environment vars)
    {
        throw new RuntimeException("Tried to evaluate binary");
    }

    /**
     * Gets the left hand side of the expression
     */
    public SymbolicExpression getLhs()
    {
        return lhs;
    }
    
    /**
     * Gets the right hand side of the expression
     */
    public SymbolicExpression getRhs()
    {
       return rhs;
    }

     /**
     * Sets the left hand side of the expression
     */
    public void setLhs(SymbolicExpression lhs)
    {
        this.lhs = lhs;
    }

     /**
     * Sets the right hand side of the expression
     */
    public void setRhs(SymbolicExpression rhs)
    {
       this.rhs = rhs;
    }
    
    /**
     * Returns the expression in string format
     */
    public String toString() {
        if(lhs.getPriority() > this.getPriority() && rhs.getPriority() > this.getPriority())
        {
            return "("+this.lhs.toString() + ") " + this.getName() + " (" + this.rhs.toString() + ")";
        }
        else if(lhs.getPriority() > this.getPriority())
        {
            return "("+this.lhs.toString() + ") " + this.getName() + " " + this.rhs.toString();
        }
        else if(rhs.getPriority() > this.getPriority())
        {
            return this.lhs.toString() + " " + this.getName() + " (" + this.rhs.toString() + ")";
        }
        else
        {
            return this.lhs.toString() + " " + this.getName() + " " + this.rhs.toString();
        }
    }
}
