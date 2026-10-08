package org.ioopm.calculator;

import org.ioopm.calculator.ast.*;
import org.ioopm.calculator.parser.CalculatorParser;

import static org.junit.jupiter.api.Assertions.*;

import java.io.IOException;

import org.junit.jupiter.api.AfterAll;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

public class CalcTests {
    static Environment vars = new Environment();
    static CalculatorParser calculatorParser = new CalculatorParser();
    Constant c1 = new Constant(5);
    Constant c0 = new Constant(15);
    Constant c3 = new Constant(10);
    Constant c2 = new Constant(5);
    Addition add = new Addition(c1, c2);
    Multiplication add2 = new Multiplication(new Constant(3), new Addition(c1, add));
    Subtraction sub = new Subtraction(c1, c2);
    Multiplication multi = new Multiplication(c1, c2);
    Multiplication multi2 = new Multiplication(new Addition(c1, c2), new Addition(c1, c2));
    Multiplication multi3 = new Multiplication(new Multiplication(c1, c2), new Multiplication(c1, c2));
    Multiplication multi33 = new Multiplication(new Multiplication(c1, c2), new Multiplication(c1, c2));

    Addition add11 = new Addition(new Addition(c1, c2), new Constant(5));
    Addition add22 = new Addition(new Addition(c1, c2), new Constant(5));

    Division div = new Division(c1, c2);
    Variable v = new Variable("x");
    Addition a = new Addition(c1, v);
    Multiplication m = new Multiplication(a, c2);

    @BeforeAll
    static void initAll() {
    }

    @BeforeEach
    void init() {
    }

    @Test
    void succeedingTest() {
        assertTrue(true);
    }

    @Test
    void testPrinting() {
        assertTrue("5.0".equals("" + c1));
        assertTrue("5.0 + 5.0".equals("" + add));
        assertTrue("5.0 - 5.0".equals("" + new Subtraction(new Constant(5), new Constant(5))));
        assertTrue("5.0 * 5.0".equals("" + new Multiplication(new Constant(5), new Constant(5))));
        assertTrue("5.0 / 5.0".equals("" + div));
        assertTrue("5.0 + x".equals("" + a));
        assertTrue("x".equals("" + v));
        Multiplication mult = new Multiplication(new Addition(new Constant(5), new Variable("x")), new Constant(5));
        assertTrue("(5.0 + x) * 5.0".equals("" + mult));
        assertTrue("(5.0 + 5.0) * (5.0 + 5.0)"
                .equals("" + new Multiplication(new Addition(new Constant(5), new Constant(5)),
                        new Addition(new Constant(5), new Constant(5)))));
        assertTrue("5.0 * 5.0 * 5.0 * 5.0".equals("" + multi3));
    }

    @Test
    void testEvaluating() {
        assertTrue(add2.eval(vars).equals(new Constant(45)));
        assertTrue(new Addition(new Variable("x"), new Constant(37)).eval(vars)
                .equals(new Addition(new Variable("x"), new Constant(37)).eval(vars)));
        assertTrue(new Cos(new Constant(0)).equals(new Cos(new Constant(0))));
        new Assignment(new Addition(c1, c0), new Variable("x"), vars).eval(vars);
        assertTrue(vars.containsKey(new Variable("x")));
        System.out.println(vars.get(new Variable("x")).toString());
        new Assignment(new Addition(new Variable("z"), c1), new Variable("y"), vars);
        assertTrue(vars.get(new Variable("x")).equals(new Constant(20)));
        // assertTrue(vars.get(new Variable("x")).equals(new Constant(20)));
    }

    @Test
    void testConstants() {
        try {
            new Assignment(new Constant(3), new Variable("pi"), vars);
        } catch (IllegalExpressionException se) {
            succeedingTest();
        }
        assertTrue(Constants.namedConstants.containsKey("pi"));
        assertTrue(Constants.namedConstants.get("pi").equals(Math.PI));
    }

    @Test
    void testAddition() { // TODO, lägg till toString
        Addition add1 = new Addition(new Constant(3), new Constant(5));
        Addition add2 = new Addition(new Constant(3), new Constant(-5));
        try {
            add1.getValue();
        } catch (RuntimeException re) {
            succeedingTest();
        }
        assertFalse(add1.isConstant());
        assertFalse(add1.isCommand());
        assertEquals(2, add1.getPriority());
        assert (add1.eval(vars).equals(new Constant(8)));
        assert (add2.eval(vars).equals(new Constant(-2)));
        assert (add2.equals(new Addition(new Constant(3), new Constant(-5))));
        assertEquals(add2.toString(), "3.0 + -5.0");
        assertFalse(add2.equals(new Addition(new Constant(-5), new Constant(3))));
    }

    @Test
    void testAssignment() {
        Assignment assign1 = new Assignment(new Constant(3), new Variable("x"), vars);
        Assignment assign2 = new Assignment(new Constant(3), new Variable("x"), vars);
        Assignment assign3 = new Assignment(new Constant(4), new Variable("x"), vars);
        try {
            assign1.getValue();
        } catch (RuntimeException re) {
            succeedingTest();
        }
        assertFalse(assign1.isConstant());
        assertFalse(assign1.isCommand());
        assertEquals(0, assign1.getPriority());
        assert (assign1.eval(vars).equals(new Constant(3)));
        assertEquals(assign1.toString(), "3.0 = x");
        assert (assign1.equals(assign2));
        assertFalse(assign1.equals(assign3));
    }

    @Test
    void testConstant() {
        Constant const1 = new Constant(1);
        Constant const2 = new Constant(1);
        Constant const3 = new Constant(-1);
        try {
            const1.getValue();
        } catch (RuntimeException re) {
            succeedingTest();
        }
        assertTrue(const1.isConstant());
        assertFalse(const1.isCommand());
        assertEquals(0, const1.getPriority());
        assert (const1.eval(vars).equals(const2));
        assertEquals(const1.toString(), "1.0");
        assert (const1.equals(const2));
        assertEquals(const3.eval(vars), const3);
    }

    @Test
    void testCos() {
        Constant const1 = new Constant(0);
        Cos cos1 = new Cos(const1);
        try {
            cos1.getValue();
        } catch (RuntimeException re) {
            succeedingTest();
        }
        assertFalse(cos1.isConstant());
        assertFalse(cos1.isCommand());
        assertEquals(0, cos1.getPriority());
        assertEquals(cos1.toString(), "cos(0.0)");
        assert (cos1.eval(vars).equals(new Constant(1)));
        assert (cos1.equals(new Cos(new Constant(0))));
    }

    @Test
    void testDivision() {
        Division div1 = new Division(new Constant(-3), new Constant(5));
        Division div2 = new Division(new Constant(3), new Constant(-5));
        try {
            div1.getValue();
        } catch (RuntimeException re) {
            succeedingTest();
        }
        assertFalse(div1.isConstant());
        assertFalse(div2.isCommand());
        assertEquals(1, div1.getPriority());
        assert (div1.eval(vars).equals(new Constant(-0.6)));
        assert (div2.eval(vars).equals(div1.eval(vars)));
        assert (div2.equals(new Division(new Constant(3), new Constant(-5))));
        assertFalse(div2.equals(div1));
    }

    @Test
    void testExp() {
        Exp exp1 = new Exp(new Constant(0));
        Exp exp2 = new Exp(new Constant(-1));
        try {
            exp1.getValue();
        } catch (RuntimeException re) {
            succeedingTest();
        }
        assertFalse(exp1.isConstant());
        assertFalse(exp1.isCommand());
        assertEquals(0, exp1.getPriority());
        assert (exp1.eval(vars).equals(new Constant(1)));
        assert (exp1.equals(new Exp(new Constant(0))));
        assert (exp2.eval(vars).equals(new Constant(1 / Math.E)));
        assert (exp2.equals(new Exp(new Constant(-1))));
    }

    @Test
    void testLog() {
        Log log1 = new Log(new Constant(Math.E));
        Log log2 = new Log(new Constant(1 / Math.E));
        try {
            log1.getValue();
        } catch (RuntimeException re) {
            succeedingTest();
        }
        assertFalse(log1.isConstant());
        assertFalse(log1.isCommand());
        assertEquals(0, log1.getPriority());
        assert (log1.eval(vars).equals(new Constant(1)));
        assert (log1.equals(new Log(new Constant(Math.E))));
        assert (log2.eval(vars).equals(new Constant(-1)));
        assert (log2.equals(new Log(new Constant(1 / Math.E))));
    }

    @Test
    void testMultiplication() {
        Multiplication mult1 = new Multiplication(new Constant(-3), new Constant(5));
        Multiplication mult2 = new Multiplication(new Constant(-3), new Constant(-5));
        try {
            mult1.getValue();
        } catch (RuntimeException re) {
            succeedingTest();
        }
        assertFalse(mult1.isConstant());
        assertFalse(mult1.isCommand());
        assertEquals(1, mult1.getPriority());
        assert (mult1.eval(vars).equals(new Constant(-15)));
        assert (mult1.equals(new Multiplication(new Constant(-3), new Constant(5))));
        assert (mult2.equals(new Multiplication(new Constant(-3), new Constant(-5))));
        assert (mult2.eval(vars).equals(new Constant(15)));
    }

    @Test
    void testNegation() {
        Negation neg1 = new Negation(new Constant(1));
        Negation neg2 = new Negation(new Constant(-1));
        try {
            neg1.getValue();
        } catch (RuntimeException re) {
            succeedingTest();
        }
        assertFalse(neg1.isConstant());
        assertFalse(neg1.isCommand());
        assertEquals(0, neg1.getPriority());
        assert (neg1.eval(vars).equals(new Constant(-1)));
        assertEquals(neg1.toString(), "-1.0");
        assert (neg1.equals(new Negation(new Constant(1))));
        assert (neg2.eval(vars).equals(new Constant(1)));
        assert (neg2.equals(new Negation(new Constant(-1))));
    }

    @Test
    void testSin() {
        Sin sin1 = new Sin(new Constant(0));
        try {
            sin1.getValue();
        } catch (RuntimeException re) {
            succeedingTest();
        }
        assertFalse(sin1.isConstant());
        assertFalse(sin1.isCommand());
        assertEquals(0, sin1.getPriority());
        assertEquals(sin1.toString(), "sin(0.0)");
        assert (sin1.eval(vars).equals(new Constant(0)));
        assert (sin1.equals(new Sin(new Constant(0))));
    }

    @Test
    void testSubtraction() {
        Subtraction sub1 = new Subtraction(new Constant(3), new Constant(5));
        Subtraction sub2 = new Subtraction(new Constant(3), new Constant(-5));
        try {
            sub1.getValue();
        } catch (RuntimeException re) {
            succeedingTest();
        }
        assertFalse(sub1.isConstant());
        assertFalse(sub1.isCommand());
        assertEquals(2, sub1.getPriority());
        assert (sub1.eval(vars).equals(new Constant(-2)));
        assertEquals(sub1.toString(), "3.0 - 5.0");
        assert (sub2.eval(vars).equals(new Constant(8)));
        assert (sub2.equals(new Subtraction(new Constant(3), new Constant(-5))));
        assertFalse(sub2.equals(new Subtraction(new Constant(-5), new Constant(3))));
    }

    @Test
    void testVariable() {
        Variable var1 = new Variable("x");
        Variable var2 = new Variable("x");

        try {
            var1.getValue();
        } catch (RuntimeException re) {
            succeedingTest();
        }

        assertFalse(var1.isConstant());
        assertFalse(var1.isCommand());
        vars.put(var1, new Constant(3));
        assert (var1.eval(vars).equals(new Constant(3)));
        assert (var1.equals(var2));
    }

    @Test
    void testIntegrationAST() {
        Subtraction sub1 = new Subtraction(new Constant(3), new Constant(5));
        Subtraction sub2 = new Subtraction(new Constant(3), new Constant(-5));
        Subtraction sub3 = new Subtraction(sub1, sub2);
        // assert((calculatorParser.parse(sub3.toString(),
        // vars)).toString().equals(sub3.toString()));
    }

    static SymbolicExpression parse(String input) {
        try {
            return calculatorParser.parse(input, vars);
        } catch (Exception ex) {
            System.out.println("Should not end up here");
            return null;
        }
    }

    static void runTest(SymbolicExpression e) {
        String input = e.toString();
        try {
            SymbolicExpression result = calculatorParser.parse(input, vars);
            assertEquals(e, result);
        } catch (Exception ex) {
            System.out.println("Should not end up here");
        }
    }

    static SymbolicExpression con(double d) {
        return new Constant(d);
    }

    static SymbolicExpression var(String s) {
        return new Variable(s);
    }

    static SymbolicExpression add(SymbolicExpression l, SymbolicExpression r) {
        return new Addition(l, r);
    }

    @Test
    void testConstant1() {
        runTest(con(42.0));
        runTest(con(0.0));
        runTest(con(3.14159));
        runTest(con(-3.14159));
    }

    @Test
    void testVariable1() {
        runTest(var("x"));
        runTest(var("xyz"));
        runTest(var("aVeryLongVariableNameThatNeverSeemsToEnd"));
    }

    @Test
    void testAddition1() {
        runTest(add(con(1.0), con(2.0)));
        runTest(add(var("x"), con(2.0)));
        runTest(add(add(con(1.0), con(2.0)),
                add(var("Three"), var("Four"))));
    }

    @Test
    void testParentheses() throws IOException {
        assertEquals(calculatorParser.parse("(1)", vars), new Constant(1.0));
        assertEquals(calculatorParser.parse("((1))", vars), new Constant(1.0));
        assertEquals(calculatorParser.parse("(((1)))", vars), new Constant(1.0));
        assertEquals(calculatorParser.parse("(((1 + 2) + 3) + 4)", vars),
                add(add(add(con(1), con(2)), con(3)), con(4)));
        assertEquals(calculatorParser.parse("(1 + (2 + (3 + 4)))", vars),
                add(con(1), add(con(2), add(con(3), con(4)))));
        assertThrows(SyntaxErrorException.class, () -> {
            calculatorParser.parse("(1 + ", vars);
        });
    }

    @AfterEach
    void tearDown() {
    }

    @AfterAll
    static void tearDownAll() {
    }

}
