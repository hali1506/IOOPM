package org.ioopm.calculator.ast;


public abstract class Command extends SymbolicExpression{
    Command() {}
    
    public boolean isCommand(){
        return true;
    }
}
