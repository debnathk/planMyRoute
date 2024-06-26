using System;
using System.IO;
using localsolver;

public class Tsp : IDisposable
{
    // Number of cities
    int nbCities;

    // Vector of distance between two cities
    long[][] distMatrixData;

    // LocalSolver
    LocalSolver localsolver;

    // Decision variables
    LSExpression cities;

    // Objective
    LSExpression obj;

    public Tsp()
    {
        localsolver = new LocalSolver();
    }

    // Read instance data
    void ReadInstance(string fileName)
    {
        using (StreamReader input = new StreamReader(fileName))
        {
            // The input files follow the TSPLib "explicit" format
            string line;
            while ((line = input.ReadLine()) != null)
            {
                string[] splitted = line.Split(':');
                if (splitted[0].Contains("DIMENSION"))
                    nbCities = int.Parse(splitted[1]);
                else if (splitted[0].Contains("EDGE_WEIGHT_SECTION"))
                    break;
            }

            string[] matrixText = input
                .ReadToEnd()
                .Split((char[])null, StringSplitOptions.RemoveEmptyEntries);
            distMatrixData = new long[nbCities][];
            for (int i = 0; i < nbCities; ++i)
            {
                distMatrixData[i] = new long[nbCities];
                for (int j = 0; j < nbCities; ++j)
                    distMatrixData[i][j] = long.Parse(matrixText[i * nbCities + j]);
            }
        }
    }

    public void Dispose()
    {
        if (localsolver != null)
            localsolver.Dispose();
    }

    void Solve(int limit)
    {
        // Declare the optimization model
        LSModel model = localsolver.GetModel();

        // A list variable: cities[i] is the index of the ith city in the tour
        cities = model.List(nbCities);

        // All cities must be visited
        model.Constraint(model.Count(cities) == nbCities);

        // Create a LocalSolver array for the distance matrix in order to be able to access it with "at" operators
        LSExpression distMatrix = model.Array(distMatrixData);

        // Minimize the total distance
        LSExpression distLambda = model.LambdaFunction(i => distMatrix[cities[i - 1], cities[i]]);
        obj = model.Sum(model.Range(1, nbCities), distLambda) + distMatrix[cities[nbCities - 1], cities[0]];

        model.Minimize(obj);
        model.Close();

        // Parametrize the solver
        localsolver.GetParam().SetTimeLimit(limit);

        localsolver.Solve();
    }

    /* Write the solution in a file */
    void WriteSolution(string fileName)
    {
        using (StreamWriter output = new StreamWriter(fileName))
        {
            output.WriteLine(obj.GetValue());
            LSCollection citiesCollection = cities.GetCollectionValue();
            for (int i = 0; i < nbCities; ++i)
                output.Write(citiesCollection.Get(i) + " ");
            output.WriteLine();
        }
    }

    public static void Main(string[] args)
    {
        if (args.Length < 1)
        {
            Console.WriteLine("Usage: Tsp inputFile [solFile] [timeLimit]");
            Environment.Exit(1);
        }
        string instanceFile = args[0];
        string outputFile = args.Length > 1 ? args[1] : null;
        string strTimeLimit = args.Length > 2 ? args[2] : "30";

        using (Tsp model = new Tsp())
        {
            model.ReadInstance(instanceFile);
            model.Solve(int.Parse(strTimeLimit));
            if (outputFile != null)
                model.WriteSolution(outputFile);
        }
    }
}
