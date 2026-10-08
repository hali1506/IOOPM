package org.ioopm.calculator.ast;

public class NamedConstant extends Constant
{
    private String name;
    
    public NamedConstant(String name, double value)
    {
        super(value);
        this.name = name;
    }

    public String getName(){
        return this.name;
    }
}