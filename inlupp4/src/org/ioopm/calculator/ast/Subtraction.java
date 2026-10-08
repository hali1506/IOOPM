package org.ioopm.calculator.ast;


public class Subtraction extends Binary {
    /**
     * Creates a subtraction object
     * @param lhs - The left hand side of the operation
     * @param rhs - The right hand side of the operation
     */
    public Subtraction(SymbolicExpression lhs, SymbolicExpression rhs)
    {
        super(lhs, rhs);
    }
    
    /**
     * Returns the name of the operation
     */
    public String getName(){
        return "-";
    }

    /**
     * Returns the priority of the operation
     */
    public int getPriority(){
        return 2;
    }

    /**
     * Compares equality between objects
     */
    public boolean equals(Object other) {
        if (other instanceof Subtraction ) {
            return this.equals((Subtraction) other);
        } else {
            return false;
        }
    }

    public boolean equals(Subtraction other) {
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
            return new Constant(this.getLhs().getValue() - this.getRhs().getValue());
        }
        else if(this.getLhs().isConstant()){
            return new Subtraction(this.getLhs(), this.getRhs().eval(vars));
        }
        else if(this.getRhs().isConstant()){
            return new Subtraction(this.getLhs().eval(vars), this.getRhs());
        }
        else {
            return new Subtraction(this.getLhs().eval(vars), this.getRhs().eval(vars));
        }
    }
}
