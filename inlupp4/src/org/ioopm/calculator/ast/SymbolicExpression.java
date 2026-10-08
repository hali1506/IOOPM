package org.ioopm.calculator.ast; /// could place this in parser *for now*

public abstract class SymbolicExpression {
    protected int priority = 0;

    /**
     * Creates a symbolic expression objet
     */
    public SymbolicExpression() {}
    
    /**
     * 
     * @param environment - a hash map of variables
     * @return the expression
     */
    public SymbolicExpression eval(Environment environment){
        return this;
    }

    /**
     * 
     * @return false since the expression is not a nessecarily a constant
     */
    public boolean isConstant(){
        return false;
    }

    /**
     * 
     * @return false since the expression is not nessecarily a multiplication
     */
    public boolean isMultiplication(){
        return false;
    }

    /**
     * 
     * @return false since the expression is not nessecarily a variable
     */
    public boolean isVariable(){
        return false;
    }

    /**
     * 
     * @return false since the expression is not nessecarily a command
     */
    public boolean isCommand(){
        return false;
    }

    /**
     * 
     * @return the priority of the expression
     */
    public int getPriority()
    {
        return this.priority;
    }

    /**
     * 
     * Throws an exception since it should not be on a symbolic expression
     */
    public double getValue(){
        throw new RuntimeException("getValue() called on expression which is not a constant");
    } 

    /**
     * 
     * Throws an exception since it should not be on a symbolic expression
     */
    public String getName(){
        throw new RuntimeException("getName() called on expression with no operator");
    } 

    /**
     * 
     * Throws an exception since it should not be on a symbolic expression
     */
    public String getIdentifier(){
        throw new RuntimeException("getName() called on expression which is not a variable");
    }

    /**
     * Throws an exception since it should not be on a symbolic expression
     */
    public boolean equals(Object other)
    {
        throw new RuntimeException("equals() is run on SymbolicExpression or a subclass of SymbolicExpression that is missing the equals function");
    }

    /**
     * Throws an exception since it should not be on a symbolic expression
     */
    public SymbolicExpression getLhs()
    {
        throw new RuntimeException("Cannot get left hand side of a symbolic expression");
    }

    /**
     * Throws an exception since it should not be on a symbolic expression
     */
    public SymbolicExpression getRhs()
    {
        throw new RuntimeException("Cannot get the right hand side of the symbolic expression");
    } 
}
