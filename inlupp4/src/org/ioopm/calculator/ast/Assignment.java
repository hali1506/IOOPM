package org.ioopm.calculator.ast;

public class Assignment extends Binary
{
    /**
     * Creates an assignment object
     * @param lhs - The left hand side of the operation
     * @param rhs - The right hand side of the operation
     */
    public Assignment(SymbolicExpression result, SymbolicExpression key, Environment vars)
    {
        super(result, key);
    }

    /**
     * Returns true is the objects are equal and false otherwise
     * @param other - The object to compare to
     */
    public boolean equals(Object other) {
        if (other instanceof Assignment ) {
            return this.equals((Assignment) other);
        } else {
            return false;
        }
    }

    private boolean equals(Assignment other) {
        return this.getRhs().equals(other.getRhs()) && this.getLhs().equals(other.getLhs());
    }

    @Override
    public String getName() {
        return "=";
    }

    /**
     * Evaluates the expression
     * @param vars - A hash map of variables
     */
    public SymbolicExpression eval(Environment vars){
        if(this.getRhs().isVariable()){
            if(Constants.namedConstants.containsKey(this.getRhs().getIdentifier())){
                throw new IllegalExpressionException("Variable name is already a constant");
            }
            this.setLhs(this.getLhs().eval(vars));
            vars.put((Variable) this.getRhs(), this.getLhs());
            return this.getLhs();
        }
        else{
            throw new RuntimeException("Variable name must be on right hand side");
        }
    }
    
}
