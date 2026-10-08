package org.ioopm.calculator.ast;

public class Constant extends Atom
{
    private double value;

    /**
     * Creates a constant object
     * @param expression
     */
    public Constant(double value)
    {   
        if (value <= (1.2E-16) && value > 0){ //TODO, funkar inte?
            this.value = 0;
        }
        else {
            this.value = value;
        }
    }

    /**
     * Evaluates the expression
     */
    public SymbolicExpression eval(Environment environment)
    {
        return this;
    }

     /**
     * Returns true is the objects are equal and false otherwise
     * @param other - The object to compare to
     */
    public boolean equals(Object other) {
        if (other instanceof Constant) {
            return this.equals((Constant) other);
        } else {
            return false;
        }
    }
    
    private boolean equals(Constant other) {
        return this.value == other.value;
    }

    /**
     * Gets the value of the constant
     */
    public double getValue() {
        return value;
    }

    /**
     * Returns the priority of the operation
     */  
    public int getPriority(){
        return 0;
    }

    /**
     * Returns true if the expression is a constant
     */
    public boolean isConstant(){
        return true;
    }

    /**
     * Return the value of the constant in the form of a string
     */
    public String toString() {
        return String.valueOf(this.value);
    }
}

