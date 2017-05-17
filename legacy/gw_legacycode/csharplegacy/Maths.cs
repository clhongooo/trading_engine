using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections;


/******************************************************************************
                          Class LstSquQuadRegr
     A C#  Class for Least Squares Regression for Quadratic Curve Fitting
                          Alex Etchells  2010    
******************************************************************************/

//public class LstSquQuadRegr
//{
//     /* instance variables */
//    ArrayList pointArray = new ArrayList(); 
//    private int numOfEntries; 
//    private double[] pointpair;          

//    /*constructor */
//    public LstSquQuadRegr()
//    {
//        numOfEntries = 0;
//        pointpair = new double[2];
//    }

//    /*instance methods */    
//    /// <summary>
//    /// add point pairs
//    /// </summary>
//    /// <param name="x">x value</param>
//    /// <param name="y">y value</param>
//    public void AddPoints(double x, double y) 
//    {
//        pointpair = new double[2]; 
//        numOfEntries +=1; 
//        pointpair[0] = x; 
//        pointpair[1] = y;
//        pointArray.Add(pointpair);
//    }

//    /// <summary>
//    /// returns the a term of the equation ax^2 + bx + c
//    /// </summary>
//    /// <returns>a term</returns>
//    public double aTerm()
//    {
//        if (numOfEntries < 3)
//        {
//            throw new InvalidOperationException(
//               "Insufficient pairs of co-ordinates");
//        }
//        //notation sjk to mean the sum of x_i^j*y_i^k. 
//        double s40 = getSx4(); //sum of x^4
//        double s30 = getSx3(); //sum of x^3
//        double s20 = getSx2(); //sum of x^2
//        double s10 = getSx();  //sum of x
//        double s00 = numOfEntries;
//        //sum of x^0 * y^0  rbdts 1 * number of entries

//        double s21 = getSx2y(); //sum of x^2*y
//        double s11 = getSxy();  //sum of x*y
//        double s01 = getSy();   //sum of y

//        //a = Da/D
//        return (s21*(s20 * s00 - s10 * s10) - 
//                s11*(s30 * s00 - s10 * s20) + 
//                s01*(s30 * s10 - s20 * s20))
//                /
//                (s40*(s20 * s00 - s10 * s10) -
//                 s30*(s30 * s00 - s10 * s20) + 
//                 s20*(s30 * s10 - s20 * s20));
//    }

//    /// <summary>
//    /// returns the b term of the equation ax^2 + bx + c
//    /// </summary>
//    /// <returns>b term</returns>
//    public double bTerm()
//    {
//        if (numOfEntries < 3)
//        {
//            throw new InvalidOperationException(
//               "Insufficient pairs of co-ordinates");
//        }
//        //notation sjk to mean the sum of x_i^j*y_i^k.
//        double s40 = getSx4(); //sum of x^4
//        double s30 = getSx3(); //sum of x^3
//        double s20 = getSx2(); //sum of x^2
//        double s10 = getSx();  //sum of x
//        double s00 = numOfEntries;
//        //sum of x^0 * y^0  rbdts 1 * number of entries

//        double s21 = getSx2y(); //sum of x^2*y
//        double s11 = getSxy();  //sum of x*y
//        double s01 = getSy();   //sum of y

//        //b = Db/D
//        return (s40*(s11 * s00 - s01 * s10) - 
//                s30*(s21 * s00 - s01 * s20) + 
//                s20*(s21 * s10 - s11 * s20))
//                /
//                (s40 * (s20 * s00 - s10 * s10) - 
//                 s30 * (s30 * s00 - s10 * s20) + 
//                 s20 * (s30 * s10 - s20 * s20));
//    }

//    /// <summary>
//    /// returns the c term of the equation ax^2 + bx + c
//    /// </summary>
//    /// <returns>c term</returns>
//    public double cTerm()
//    {
//        if (numOfEntries < 3)
//        {
//            throw new InvalidOperationException(
//                       "Insufficient pairs of co-ordinates");
//        }
//        //notation sjk to mean the sum of x_i^j*y_i^k.
//        double s40 = getSx4(); //sum of x^4
//        double s30 = getSx3(); //sum of x^3
//        double s20 = getSx2(); //sum of x^2
//        double s10 = getSx();  //sum of x
//        double s00 = numOfEntries;
//        //sum of x^0 * y^0  rbdts 1 * number of entries

//        double s21 = getSx2y(); //sum of x^2*y
//        double s11 = getSxy();  //sum of x*y
//        double s01 = getSy();   //sum of y

//        //c = Dc/D
//        return (s40*(s20 * s01 - s10 * s11) - 
//                s30*(s30 * s01 - s10 * s21) + 
//                s20*(s30 * s11 - s20 * s21))
//                /
//                (s40 * (s20 * s00 - s10 * s10) - 
//                 s30 * (s30 * s00 - s10 * s20) + 
//                 s20 * (s30 * s10 - s20 * s20));
//    }
    
//    public double rSquare() // get r-squared
//    {
//        if (numOfEntries < 3)
//        {
//            throw new InvalidOperationException(
//               "Insufficient pairs of co-ordinates");
//        }
//        // 1 - (residual sum of squares / total sum of squares)
//        return 1 - getSSerr() / getSStot();
//    }
   

//    /*helper methods*/
//    private double getSx() // get sum of x
//    {
//        double Sx = 0;
//        foreach (double[] ppair in pointArray)
//        {
//            Sx += ppair[0];
//        }
//        return Sx;
//    }

//    private double getSy() // get sum of y
//    {
//        double Sy = 0;
//        foreach (double[] ppair in pointArray)
//        {
//            Sy += ppair[1];
//        }
//        return Sy;
//    }

//    private double getSx2() // get sum of x^2
//    {
//        double Sx2 = 0;
//        foreach (double[] ppair in pointArray)
//        {
//            Sx2 += Math.Pow(ppair[0], 2); // sum of x^2
//        }
//        return Sx2;
//    }

//    private double getSx3() // get sum of x^3
//    {
//        double Sx3 = 0;
//        foreach (double[] ppair in pointArray)
//        {
//            Sx3 += Math.Pow(ppair[0], 3); // sum of x^3
//        }
//        return Sx3;
//    }

//    private double getSx4() // get sum of x^4
//    {
//        double Sx4 = 0;
//        foreach (double[] ppair in pointArray)
//        {
//            Sx4 += Math.Pow(ppair[0], 4); // sum of x^4
//        }
//        return Sx4;
//    }

//    private double getSxy() // get sum of x*y
//    {
//        double Sxy = 0;
//        foreach (double[] ppair in pointArray)
//        {
//            Sxy += ppair[0] * ppair[1]; // sum of x*y
//        }
//        return Sxy;
//    }

//    private double getSx2y() // get sum of x^2*y
//    {
//        double Sx2y = 0;
//        foreach (double[] ppair in pointArray)
//        {
//            Sx2y += Math.Pow(ppair[0], 2) * ppair[1]; // sum of x^2*y
//        }
//        return Sx2y;
//    }

//    private double getYMean() // mean value of y
//    {
//        double y_tot = 0;
//        foreach (double[] ppair in pointArray)
//        {
//            y_tot += ppair[1]; 
//        }
//        return y_tot/numOfEntries;
//    }

//    private double getSStot() // total sum of squares
//    {
//        //the sum of the squares of the differences between 
//        //the measured y values and the mean y value
//        double ss_tot = 0;
//        foreach (double[] ppair in pointArray)
//        {
//            ss_tot += Math.Pow(ppair[1] - getYMean(), 2);
//        }
//        return ss_tot;
//    }

//    private double getSSerr() // residual sum of squares
//    {
//        //the sum of the squares of te difference between 
//        //the measured y values and the values of y predicted by the equation
//        double ss_err = 0;
//        foreach (double[] ppair in pointArray)
//        {
//            ss_err += Math.Pow(ppair[1] - getPredictedY(ppair[0]), 2);
//        }
//        return ss_err;
//    }

//    private double getPredictedY(double x)
//    {
//        //returns value of y predicted by the equation for a given value of x
//        return aTerm() * Math.Pow(x, 2) + bTerm() * x + cTerm();
//    }
//}


public class LstSquCubicRegr
{
    int iPtr = 0;
    double[] daX = new double[10000];
    double[] daY = new double[10000];
    double[,] daSimEqn = new double[4,5];

    bool bSolved = false;
    double daTerm = 0;
    double dbTerm = 0;
    double dcTerm = 0;
    double ddTerm = 0;
    public double aTerm { get { if (!bSolved) Solve(); return daTerm; } }
    public double bTerm { get { if (!bSolved) Solve(); return dbTerm; } }
    public double cTerm { get { if (!bSolved) Solve(); return dcTerm; } }
    public double dTerm { get { if (!bSolved) Solve(); return ddTerm; } }

    public void AddPoints(double x, double y)
    {
        daX[iPtr] = x;
        daY[iPtr] = y;
        iPtr += 1;
    }

    private void Solve()
    {
        if (bSolved || iPtr == 0) return;

        daSimEqn[0, 0] = get_sum_x(6);
        daSimEqn[0, 1] = get_sum_x(5);
        daSimEqn[0, 2] = get_sum_x(4);
        daSimEqn[0, 3] = get_sum_x(3);
        daSimEqn[0, 4] = get_sum_xy(3);

        daSimEqn[1, 0] = get_sum_x(5);
        daSimEqn[1, 1] = get_sum_x(4);
        daSimEqn[1, 2] = get_sum_x(3);
        daSimEqn[1, 3] = get_sum_x(2);
        daSimEqn[1, 4] = get_sum_xy(2);

        daSimEqn[2, 0] = get_sum_x(4);
        daSimEqn[2, 1] = get_sum_x(3);
        daSimEqn[2, 2] = get_sum_x(2);
        daSimEqn[2, 3] = get_sum_x(1);
        daSimEqn[2, 4] = get_sum_xy(1);

        daSimEqn[3, 0] = get_sum_x(3);
        daSimEqn[3, 1] = get_sum_x(2);
        daSimEqn[3, 2] = get_sum_x(1);
        daSimEqn[3, 3] = 1;
        daSimEqn[3, 4] = get_sum_xy(0);

        if (LinearEquationSolver.Solve(daSimEqn))
        {
            daTerm = daSimEqn[0, 4];
            dbTerm = daSimEqn[1, 4];
            dcTerm = daSimEqn[2, 4];
            ddTerm = daSimEqn[3, 4];
        }

    }

    private double get_sum_x(double dXPower)
    {
        double dRtn = 0;
        for (int i = 0; i < iPtr - 1; i++) dRtn += Math.Pow(daX[i],dXPower);
        return dRtn;
    }
    private double get_sum_xy(double dXPower)
    {
        double dRtn = 0;
        for (int i = 0; i < iPtr - 1; i++) dRtn += Math.Pow(daX[i], dXPower) * daY[i];
        return dRtn;
    }
}


public class LstSquQuarticRegr
{
    int iPtr = 0;
    double[] daX = new double[10000];
    double[] daY = new double[10000];
    double[,] daSimEqn = new double[5, 6];

    bool bSolved = false;
    double daTerm = 0;
    double dbTerm = 0;
    double dcTerm = 0;
    double ddTerm = 0;
    double deTerm = 0;
    public double aTerm { get { if (!bSolved) Solve(); return daTerm; } }
    public double bTerm { get { if (!bSolved) Solve(); return dbTerm; } }
    public double cTerm { get { if (!bSolved) Solve(); return dcTerm; } }
    public double dTerm { get { if (!bSolved) Solve(); return ddTerm; } }
    public double eTerm { get { if (!bSolved) Solve(); return deTerm; } }

    public void AddPoints(double x, double y)
    {
        daX[iPtr] = x;
        daY[iPtr] = y;
        iPtr += 1;
    }

    private void Solve()
    {
        if (bSolved || iPtr == 0) return;

        daSimEqn[0, 0] = get_sum_x(8);
        daSimEqn[0, 1] = get_sum_x(7);
        daSimEqn[0, 2] = get_sum_x(6);
        daSimEqn[0, 3] = get_sum_x(5);
        daSimEqn[0, 4] = get_sum_x(4);
        daSimEqn[0, 5] = get_sum_xy(4);

        daSimEqn[1, 0] = get_sum_x(7);
        daSimEqn[1, 1] = get_sum_x(6);
        daSimEqn[1, 2] = get_sum_x(5);
        daSimEqn[1, 3] = get_sum_x(4);
        daSimEqn[1, 4] = get_sum_x(3);
        daSimEqn[1, 5] = get_sum_xy(3);

        daSimEqn[2, 0] = get_sum_x(6);
        daSimEqn[2, 1] = get_sum_x(5);
        daSimEqn[2, 2] = get_sum_x(4);
        daSimEqn[2, 3] = get_sum_x(3);
        daSimEqn[2, 4] = get_sum_x(2);
        daSimEqn[2, 5] = get_sum_xy(2);

        daSimEqn[3, 0] = get_sum_x(5);
        daSimEqn[3, 1] = get_sum_x(4);
        daSimEqn[3, 2] = get_sum_x(3);
        daSimEqn[3, 3] = get_sum_x(2);
        daSimEqn[3, 4] = get_sum_x(1);
        daSimEqn[3, 5] = get_sum_xy(1);

        daSimEqn[4, 0] = get_sum_x(4);
        daSimEqn[4, 1] = get_sum_x(3);
        daSimEqn[4, 2] = get_sum_x(2);
        daSimEqn[4, 3] = get_sum_x(1);
        daSimEqn[4, 4] = 1;
        daSimEqn[4, 5] = get_sum_xy(0);
        
        
        if (LinearEquationSolver.Solve(daSimEqn))
        {
            daTerm = daSimEqn[0, 5];
            dbTerm = daSimEqn[1, 5];
            dcTerm = daSimEqn[2, 5];
            ddTerm = daSimEqn[3, 5];
            deTerm = daSimEqn[4, 5];
        }

    }

    private double get_sum_x(double dXPower)
    {
        double dRtn = 0;
        for (int i = 0; i < iPtr - 1; i++) dRtn += Math.Pow(daX[i], dXPower);
        return dRtn;
    }
    private double get_sum_xy(double dXPower)
    {
        double dRtn = 0;
        for (int i = 0; i < iPtr - 1; i++) dRtn += Math.Pow(daX[i], dXPower) * daY[i];
        return dRtn;
    }
}


public class LstSquQuinticRegr
{
    int iPtr = 0;
    double[] daX = new double[10000];
    double[] daY = new double[10000];
    double[,] daSimEqn = new double[6, 7];

    bool bSolved = false;
    double daTerm = 0;
    double dbTerm = 0;
    double dcTerm = 0;
    double ddTerm = 0;
    double deTerm = 0;
    double dfTerm = 0;
    public double aTerm { get { if (!bSolved) Solve(); return daTerm; } }
    public double bTerm { get { if (!bSolved) Solve(); return dbTerm; } }
    public double cTerm { get { if (!bSolved) Solve(); return dcTerm; } }
    public double dTerm { get { if (!bSolved) Solve(); return ddTerm; } }
    public double eTerm { get { if (!bSolved) Solve(); return deTerm; } }
    public double fTerm { get { if (!bSolved) Solve(); return dfTerm; } }

    public void AddPoints(double x, double y)
    {
        daX[iPtr] = x;
        daY[iPtr] = y;
        iPtr += 1;
    }

    private void Solve()
    {
        if (bSolved || iPtr == 0) return;

        daSimEqn[0, 0] = get_sum_x(10);
        daSimEqn[0, 1] = get_sum_x(9);
        daSimEqn[0, 2] = get_sum_x(8);
        daSimEqn[0, 3] = get_sum_x(7);
        daSimEqn[0, 4] = get_sum_x(6);
        daSimEqn[0, 5] = get_sum_x(5);
        daSimEqn[0, 6] = get_sum_xy(5);

        daSimEqn[1, 0] = get_sum_x(9);
        daSimEqn[1, 1] = get_sum_x(8);
        daSimEqn[1, 2] = get_sum_x(7);
        daSimEqn[1, 3] = get_sum_x(6);
        daSimEqn[1, 4] = get_sum_x(5);
        daSimEqn[1, 5] = get_sum_x(4);
        daSimEqn[1, 6] = get_sum_xy(4);

        daSimEqn[2, 0] = get_sum_x(8);
        daSimEqn[2, 1] = get_sum_x(7);
        daSimEqn[2, 2] = get_sum_x(6);
        daSimEqn[2, 3] = get_sum_x(5);
        daSimEqn[2, 4] = get_sum_x(4);
        daSimEqn[2, 5] = get_sum_x(3);
        daSimEqn[2, 6] = get_sum_xy(3);

        daSimEqn[3, 0] = get_sum_x(7);
        daSimEqn[3, 1] = get_sum_x(6);
        daSimEqn[3, 2] = get_sum_x(5);
        daSimEqn[3, 3] = get_sum_x(4);
        daSimEqn[3, 4] = get_sum_x(3);
        daSimEqn[3, 5] = get_sum_x(2);
        daSimEqn[3, 6] = get_sum_xy(2);

        daSimEqn[4, 0] = get_sum_x(6);
        daSimEqn[4, 1] = get_sum_x(5);
        daSimEqn[4, 2] = get_sum_x(4);
        daSimEqn[4, 3] = get_sum_x(3);
        daSimEqn[4, 4] = get_sum_x(2);
        daSimEqn[4, 5] = get_sum_x(1);
        daSimEqn[4, 6] = get_sum_xy(1);

        daSimEqn[5, 0] = get_sum_x(5);
        daSimEqn[5, 1] = get_sum_x(4);
        daSimEqn[5, 2] = get_sum_x(3);
        daSimEqn[5, 3] = get_sum_x(2);
        daSimEqn[5, 4] = get_sum_x(1);
        daSimEqn[5, 5] = 1;
        daSimEqn[5, 6] = get_sum_xy(0);


        if (LinearEquationSolver.Solve(daSimEqn))
        {
            daTerm = daSimEqn[0, 6];
            dbTerm = daSimEqn[1, 6];
            dcTerm = daSimEqn[2, 6];
            ddTerm = daSimEqn[3, 6];
            deTerm = daSimEqn[4, 6];
            dfTerm = daSimEqn[5, 6];
        }

    }

    private double get_sum_x(double dXPower)
    {
        double dRtn = 0;
        for (int i = 0; i < iPtr - 1; i++) dRtn += Math.Pow(daX[i], dXPower);
        return dRtn;
    }
    private double get_sum_xy(double dXPower)
    {
        double dRtn = 0;
        for (int i = 0; i < iPtr - 1; i++) dRtn += Math.Pow(daX[i], dXPower) * daY[i];
        return dRtn;
    }
}

public class LstSquSexticRegr
{
    int iPtr = 0;
    double[] daX = new double[10000];
    double[] daY = new double[10000];
    double[,] daSimEqn = new double[7, 8];

    bool bSolved = false;
    double daTerm = 0;
    double dbTerm = 0;
    double dcTerm = 0;
    double ddTerm = 0;
    double deTerm = 0;
    double dfTerm = 0;
    double dgTerm = 0;
    public double aTerm { get { if (!bSolved) Solve(); return daTerm; } }
    public double bTerm { get { if (!bSolved) Solve(); return dbTerm; } }
    public double cTerm { get { if (!bSolved) Solve(); return dcTerm; } }
    public double dTerm { get { if (!bSolved) Solve(); return ddTerm; } }
    public double eTerm { get { if (!bSolved) Solve(); return deTerm; } }
    public double fTerm { get { if (!bSolved) Solve(); return dfTerm; } }
    public double gTerm { get { if (!bSolved) Solve(); return dgTerm; } }

    public void AddPoints(double x, double y)
    {
        daX[iPtr] = x;
        daY[iPtr] = y;
        iPtr += 1;
    }

    private void Solve()
    {
        if (bSolved || iPtr == 0) return;

        daSimEqn[0, 0] = get_sum_x(12);
        daSimEqn[0, 1] = get_sum_x(11);
        daSimEqn[0, 2] = get_sum_x(10);
        daSimEqn[0, 3] = get_sum_x(9);
        daSimEqn[0, 4] = get_sum_x(8);
        daSimEqn[0, 5] = get_sum_x(7);
        daSimEqn[0, 6] = get_sum_x(6);
        daSimEqn[0, 7] = get_sum_xy(6);

        daSimEqn[1, 0] = get_sum_x(11);
        daSimEqn[1, 1] = get_sum_x(10);
        daSimEqn[1, 2] = get_sum_x(9);
        daSimEqn[1, 3] = get_sum_x(8);
        daSimEqn[1, 4] = get_sum_x(7);
        daSimEqn[1, 5] = get_sum_x(6);
        daSimEqn[1, 6] = get_sum_x(5);
        daSimEqn[1, 7] = get_sum_xy(5);

        daSimEqn[2, 0] = get_sum_x(10);
        daSimEqn[2, 1] = get_sum_x(9);
        daSimEqn[2, 2] = get_sum_x(8);
        daSimEqn[2, 3] = get_sum_x(7);
        daSimEqn[2, 4] = get_sum_x(6);
        daSimEqn[2, 5] = get_sum_x(5);
        daSimEqn[2, 6] = get_sum_x(4);
        daSimEqn[2, 7] = get_sum_xy(4);

        daSimEqn[3, 0] = get_sum_x(9);
        daSimEqn[3, 1] = get_sum_x(8);
        daSimEqn[3, 2] = get_sum_x(7);
        daSimEqn[3, 3] = get_sum_x(6);
        daSimEqn[3, 4] = get_sum_x(5);
        daSimEqn[3, 5] = get_sum_x(4);
        daSimEqn[3, 6] = get_sum_x(3);
        daSimEqn[3, 7] = get_sum_xy(3);

        daSimEqn[4, 0] = get_sum_x(8);
        daSimEqn[4, 1] = get_sum_x(7);
        daSimEqn[4, 2] = get_sum_x(6);
        daSimEqn[4, 3] = get_sum_x(5);
        daSimEqn[4, 4] = get_sum_x(4);
        daSimEqn[4, 5] = get_sum_x(3);
        daSimEqn[4, 6] = get_sum_x(2);
        daSimEqn[4, 7] = get_sum_xy(2);

        daSimEqn[5, 0] = get_sum_x(7);
        daSimEqn[5, 1] = get_sum_x(6);
        daSimEqn[5, 2] = get_sum_x(5);
        daSimEqn[5, 3] = get_sum_x(4);
        daSimEqn[5, 4] = get_sum_x(3);
        daSimEqn[5, 5] = get_sum_x(2);
        daSimEqn[5, 6] = get_sum_x(1);
        daSimEqn[5, 7] = get_sum_xy(1);

        daSimEqn[6, 0] = get_sum_x(6);
        daSimEqn[6, 1] = get_sum_x(5);
        daSimEqn[6, 2] = get_sum_x(4);
        daSimEqn[6, 3] = get_sum_x(3);
        daSimEqn[6, 4] = get_sum_x(2);
        daSimEqn[6, 5] = get_sum_x(1);
        daSimEqn[6, 6] = 1;
        daSimEqn[6, 7] = get_sum_xy(0);



        if (LinearEquationSolver.Solve(daSimEqn))
        {
            daTerm = daSimEqn[0, 7];
            dbTerm = daSimEqn[1, 7];
            dcTerm = daSimEqn[2, 7];
            ddTerm = daSimEqn[3, 7];
            deTerm = daSimEqn[4, 7];
            dfTerm = daSimEqn[5, 7];
            dgTerm = daSimEqn[6, 7];
        }

    }

    private double get_sum_x(double dXPower)
    {
        double dRtn = 0;
        for (int i = 0; i < iPtr - 1; i++) dRtn += Math.Pow(daX[i], dXPower);
        return dRtn;
    }
    private double get_sum_xy(double dXPower)
    {
        double dRtn = 0;
        for (int i = 0; i < iPtr - 1; i++) dRtn += Math.Pow(daX[i], dXPower) * daY[i];
        return dRtn;
    }
}




public static class LinearEquationSolver
{
    /// <summary>Computes the solution of a linear equation system.</summary>
    /// <param name="M">
    /// The system of linear equations as an augmented matrix[row, col] where (rows + 1 == cols).
    /// It will contain the solution in "row canonical form" if the function returns "true".
    /// </param>
    /// <returns>Returns whether the matrix has a unique solution or not.</returns>
    public static bool Solve(double[,] M)
    {
        // input checks
        int rowCount = M.GetUpperBound(0) + 1;
        if (M == null || M.Length != rowCount * (rowCount + 1))
            throw new ArgumentException("The algorithm must be provided with a (n x n+1) matrix.");
        if (rowCount < 1)
            throw new ArgumentException("The matrix must at least have one row.");

        // pivoting
        for (int col = 0; col + 1 < rowCount; col++) if (M[col, col] == 0)
            // check for zero coefficients
            {
                // find non-zero coefficient
                int swapRow = col + 1;
                for (; swapRow < rowCount; swapRow++) if (M[swapRow, col] != 0) break;

                if (M[swapRow, col] != 0) // found a non-zero coefficient?
                {
                    // yes, then swap it with the above
                    double[] tmp = new double[rowCount + 1];
                    for (int i = 0; i < rowCount + 1; i++)
                    { tmp[i] = M[swapRow, i]; M[swapRow, i] = M[col, i]; M[col, i] = tmp[i]; }
                }
                else return false; // no, then the matrix has no unique solution
            }

        // elimination
        for (int sourceRow = 0; sourceRow + 1 < rowCount; sourceRow++)
        {
            for (int destRow = sourceRow + 1; destRow < rowCount; destRow++)
            {
                double df = M[sourceRow, sourceRow];
                double sf = M[destRow, sourceRow];
                for (int i = 0; i < rowCount + 1; i++)
                    M[destRow, i] = M[destRow, i] * df - M[sourceRow, i] * sf;
            }
        }

        // back-insertion
        for (int row = rowCount - 1; row >= 0; row--)
        {
            double f = M[row, row];
            if (f == 0) return false;

            for (int i = 0; i < rowCount + 1; i++) M[row, i] /= f;
            for (int destRow = 0; destRow < row; destRow++)
            { M[destRow, rowCount] -= M[destRow, row] * M[row, rowCount]; M[destRow, row] = 0; }
        }
        return true;
    }
}