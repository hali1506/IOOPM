package org.ioopm.calculator.ast;

public class Variable extends Atom
{
    public String identifier;

    /**
     * Creates a variable object
     * @param sval
     */
    public Variable(String sval)
    {
        this.identifier = sval;
    }
    
    /**
     * Gets the variable name
     */
    public String getIdentifier(){
        return this.identifier;
    }

    /**
     * Returns the name of the variable in string format
     */
    public String toString(){
        return this.identifier;
    }

    /**
     * Returns true since the expression is a variable
     */
    public boolean isVariable(){
        return true;
    }

    /**
     * Returns true if the objects are equal and false otherwise
     */
    public boolean equals(Object other) {
        if (other instanceof Variable) {
            return this.equals((Variable) other);
        } else {
            return false;
        }
    }
    
    private boolean equals(Variable other) {
        return this.identifier.equals(other.identifier);
    }

    /**
     * Creates a hash code of the object
     */
    public int hashCode(){
        return this.identifier.hashCode();
    }

    /**
     * Evaluates the expression
     */
    public SymbolicExpression eval(Environment vars)
    {
        if(vars.containsKey(this)){
            return vars.get(new Variable(this.identifier));
        }
        else {
            return this;
        }
    }
}
