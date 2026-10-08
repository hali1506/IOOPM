package org.ioopm.calculator.ast;

public class Vars extends Command
{
    private static final Vars theInstance = new Vars();

    /**
     * Creates the variables object
     */
    private Vars() {}

    /**
     * 
     * @return the variable object
     */
    public static Vars instance() {
        return theInstance;
    }
}