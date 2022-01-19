using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Queen : MonoBehaviour
{
    public static bool hasWon = false;
    public bool isLeft = false;
    private static int[,] grid = new int[10,10];
    public static bool isLeftTurn = false;
    public static bool isShooting = false;
    private static Queen[] queens = new Queen[8];
    private static GameObject[,] gridSpaces = new GameObject[10,10];
    private static List<Vector2Int> visibleSquares = new List<Vector2Int>();
    public static GameObject selected = null;
    private Vector3 startPos;
    // Start is called before the first frame update
    Vector2Int TranslationToGrid(Vector3 v)
    {
        return new Vector2Int
            (
            Mathf.RoundToInt(v.x + 4.5f), //col x
            Mathf.RoundToInt(4.5f - v.z)  //row y
            );
    }
    Vector2Int TranslationToGrid()
    {
        Vector3 v = gameObject.GetComponent<Transform>().position;
        return new Vector2Int
            (
            Mathf.RoundToInt(v.x + 4.5f), //col x
            Mathf.RoundToInt(4.5f - v.z)  //row y
            );
    }
    public static void CheckWin()
    {
        foreach (Queen q in queens)
            if (GetScope(q) != 0 && isLeftTurn == q.isLeft) return;
        GameObject.Find("WorldText").GetComponent<TextMesh>().text = (isLeftTurn ? "Right Player Wins!" : "Left Player Wins!");
        hasWon = true;
    }
    public static void Shoot(Vector2Int pos)
    {
        isShooting = false;
        grid[pos.x, pos.y] = 3; //magic number for shot square
        gridSpaces[pos.x, pos.y].GetComponent<Square>().isShot = true;
        RemoveHighlight();
        //gridSpaces[pos.x, pos.y].GetComponent<Square>().InvertSelected();
        isLeftTurn = !isLeftTurn;
        GameObject.Find("WorldText").GetComponent<TextMesh>().text = (isLeftTurn ? "Left Player's Turn" : "Right Player's Turn");
        CheckWin();
    }

    public static void RemoveHighlight()
    {
        foreach (var i in visibleSquares)
            gridSpaces[i.x, i.y].GetComponent<Square>().InvertSelected();
    }

    public static void Move(Vector2Int pos)
    {
        Vector2Int Oldpos = selected.GetComponent<Queen>().TranslationToGrid();
        grid[Oldpos.x, Oldpos.y] = 0; //col x , row y
        selected.GetComponent<Transform>().position = new Vector3(pos.x - 4.5f, 0.25f, 4.5f - pos.y);
        grid[pos.x, pos.y] = selected.GetComponent<Queen>().isLeft ? 1 : 2; //col x , row y

        RemoveHighlight();
        HighlightVisibleSquares(pos);
        isShooting = true;
        selected = null;
    }

    void Start()
    {
        startPos = gameObject.GetComponent<Transform>().position;
        Vector2Int pos = TranslationToGrid();
        grid[pos.x, pos.y] = isLeft ? 1 : 2; //col x , row y

        //HACK
        if (gameObject.name == "R0")
        {
            for (int i = 0; i < 100; ++i)
                //first digit col, second digit row
                gridSpaces[i / 10, i % 10] = GameObject.Find(i < 10 ? "0" + i.ToString() : i.ToString());

            queens = GameObject.FindObjectsOfType<Queen>();
        }


    }


    public static void HighlightVisibleSquares(Vector2Int pos)
    {
        visibleSquares.Clear();

        visibleSquares = GetSpacesFrom(pos);

        foreach (var i in visibleSquares)
            gridSpaces[i.x, i.y].GetComponent<Square>().InvertSelected();
    }

    void OnMouseDown()
    {
        if (hasWon)
            return;

        if(isLeft == isLeftTurn && !isShooting)
        {
            if(selected != null)
                RemoveHighlight();
            selected = gameObject;
            Vector2Int v = TranslationToGrid();

            HighlightVisibleSquares(v);
        }
    }

    static public void ResetBoard()
    {
        if (queens[0] == null)
            return;
        hasWon = false;
        if (selected != null)
        {
            RemoveHighlight();
        }
        for (int i = 0; i < 100; ++i)
            grid[i / 10, i % 10] = 0;
        foreach (Queen q in queens)
        {
            q.GetComponent<Transform>().position = q.startPos;
            Vector2Int pos = q.TranslationToGrid();
            grid[pos.x, pos.y] = q.isLeft ? 1 : 2; //col x , row y
        }
        foreach (GameObject g in gridSpaces)
        {
            Square s= g.GetComponent<Square>();
            s.isShot = false;
            s.isHighlighted = true;
            s.InvertSelected();
        }
        isLeftTurn = false;
        GameObject.Find("WorldText").GetComponent<TextMesh>().text = "Right Player's Turn";

    }

    ////////////////////////////////////////// AI STUFF BELOW HERE //////////////////////////////////////////
    static public bool AILeft = true;
    static public bool AIRight = true;
    static public bool AILeftRandom = true;
    static public bool AIRightRandom = true;
    static public int rightDepth = 2;
    static public int leftDepth = 2;

    public bool isBoss = false;
    static public bool isRightMinimizingOponent = true;
    static public bool isLeftMinimizingOponent = true;
    static private Vector2Int toMove = new Vector2Int();
    static private Vector2Int toShoot = new Vector2Int();

    public static int GetScope(Queen q)
    {
        int count = 0;
        var pos = q.TranslationToGrid();
        for (int i = -1; i <= 1; ++i) //i for col
        {
            for (int j = -1; j <= 1; ++j) //j for row
            {
                if (i == 0 && j == 0)
                    continue;
                int offsetX = 0; //col offset
                int offsetY = 0; //row offset
                while (true)
                {
                    int testX = pos.x + (offsetX += i);
                    int testY = pos.y + (offsetY += j);
                    if (testX >= 0 && testX < 10 && testY >= 0 && testY < 10)
                    {
                        if (grid[testX, testY] == 0)
                            ++count;
                        else
                            break;
                    }
                    else
                        break;
                }
            }
        }
        return count;
    }
    public static Vector2Int GetScopes()
    {
        var result = new Vector2Int();
        foreach(Queen q in queens)
        {
            if (q.isLeft)
                result.x += GetScope(q);
            else
                result.y += GetScope(q);
        }
        return result;
    }

    public static List<Vector2Int> GetSpacesFrom(Vector2Int pos)
    {
        List<Vector2Int> result = new List<Vector2Int>();
        for (int i = -1; i <= 1; ++i) //i for col
        {
            for (int j = -1; j <= 1; ++j) //j for row
            {
                if (i == 0 && j == 0)
                    continue;
                int offsetX = 0; //col offset
                int offsetY = 0; //row offset
                while (true)
                {
                    int testX = pos.x + (offsetX += i);
                    int testY = pos.y + (offsetY += j);
                    if (testX >= 0 && testX < 10 && testY >= 0 && testY < 10)
                    {
                        if (grid[testX, testY] == 0)
                            result.Add(new Vector2Int(testX, testY));
                        else
                            break;
                    }
                    else
                        break;
                }
            }
        }
        return result;
    }

    public static bool minMax = true;

    static private bool gtfo = false;

    public static bool FindOptimalTurn(int depth, bool fakingLeftTurn)
    {
        if (depth < 0 || gtfo)
            return false;

        foreach (Queen q in queens)
        {
            if(q.isLeft == fakingLeftTurn)
            {
                GameObject fakeSelected = q.gameObject;
                Vector3 oldPosWorld = fakeSelected.GetComponent<Transform>().position;
                Vector2Int Oldpos = fakeSelected.GetComponent<Queen>().TranslationToGrid();
                var moves = GetSpacesFrom(q.TranslationToGrid());
                foreach (Vector2Int move in moves)
                {
                    grid[Oldpos.x, Oldpos.y] = 0; //col x , row y
                    fakeSelected.GetComponent<Transform>().position = new Vector3(move.x - 4.5f, 0.25f, 4.5f - move.y);
                    grid[move.x, move.y] = fakeSelected.GetComponent<Queen>().isLeft ? 1 : 2; //col x , row y

                    //////////////////////////////////////////////////////////////////////
                    var shots = GetSpacesFrom(toMove);
                    foreach (Vector2Int shot in shots)
                    {
                        grid[shot.x, shot.y] = 3; //col x , row y

                        bool winningMove = true;
                        foreach (Queen q2 in queens)
                            if (GetScope(q2) != 0 && fakingLeftTurn == q2.isLeft)
                            {
                                winningMove = false;
                                break;
                            }
                        if (winningMove && (isLeftTurn != fakingLeftTurn))
                        {

                            grid[shot.x, shot.y] = 0; //col x , row y
                            grid[Oldpos.x, Oldpos.y] = grid[move.x, move.y];
                            grid[move.x, move.y] = 0;
                            fakeSelected.GetComponent<Transform>().position = oldPosWorld;
                            toMove = move;
                            toShoot = shot;
                            gtfo = true;
                            return true;
                        }
                        if (FindOptimalTurn(depth - 1, !fakingLeftTurn))
                        {
                            grid[shot.x, shot.y] = 0; //col x , row y
                            grid[Oldpos.x, Oldpos.y] = grid[move.x, move.y];
                            grid[move.x, move.y] = 0;
                            fakeSelected.GetComponent<Transform>().position = oldPosWorld;
                            toMove = move;
                            toShoot = shot;
                            gtfo = true;
                            return true;
                        }
                        else if(depth == 0)
                        {
                            grid[shot.x, shot.y] = 0; //col x , row y
                            grid[Oldpos.x, Oldpos.y] = grid[move.x, move.y];
                            grid[move.x, move.y] = 0;
                            fakeSelected.GetComponent<Transform>().position = oldPosWorld;
                            //gtfo = true;
                            return false;
                        }

                        grid[shot.x, shot.y] = 0; //col x , row y
                    }

                    //////////////////////////////////////////////////////////////////////

                    grid[Oldpos.x, Oldpos.y] = grid[move.x, move.y];
                    grid[move.x, move.y] = 0;
                    fakeSelected.GetComponent<Transform>().position = oldPosWorld;
                }
            }
        }
        return false; //no winning moves found

    }


        public static void SwitchToScopeBased()
    {


        List<Vector2Int>[] moves = { new List<Vector2Int>(), new List<Vector2Int>(), new List<Vector2Int>(), new List<Vector2Int>() };
        Queen[] ownedQueens = { null, null, null, null };
        int i = 0;
        foreach (var q in queens)
        {
            if (q.isLeft == isLeftTurn)
            {
                ownedQueens[i] = q;
                moves[i++].AddRange(GetSpacesFrom(q.TranslationToGrid()));
            }
        }

        int bestScope = -1000;
        int bestScopesOpponentScope = 1000;
        int bestQueen = 0;

        for (i = 0; i < 4; ++i)
        {
            selected = ownedQueens[i].gameObject;
            Vector3 oldPosWorld = selected.GetComponent<Transform>().position;
            Vector2Int Oldpos = selected.GetComponent<Queen>().TranslationToGrid();
            foreach (Vector2Int move in moves[i])
            {
                grid[Oldpos.x, Oldpos.y] = 0; //col x , row y
                selected.GetComponent<Transform>().position = new Vector3(move.x - 4.5f, 0.25f, 4.5f - move.y);
                grid[move.x, move.y] = selected.GetComponent<Queen>().isLeft ? 1 : 2; //col x , row y

                //left is x and right is y
                Vector2Int scopes = GetScopes();
                int scope =         isLeftTurn ? scopes.x : scopes.y;
                int opponentScope = isLeftTurn ? scopes.y : scopes.x;

                grid[Oldpos.x, Oldpos.y] = grid[move.x, move.y];
                grid[move.x, move.y] = 0;
                selected.GetComponent<Transform>().position = oldPosWorld;

                if(minMax)
                {
                    if(scope > bestScope || (scope == bestScope && opponentScope < bestScopesOpponentScope))
                    {
                        bestQueen = i;
                        bestScope = scope;
                        bestScopesOpponentScope = opponentScope;
                        toMove = move;
                    }
                }
                else//max/min
                {
                    if (opponentScope < bestScopesOpponentScope || (scope == bestScope && scope > bestScope))
                    {
                        bestQueen = i;
                        bestScope = scope;
                        bestScopesOpponentScope = opponentScope;
                        toMove = move;
                    }
                }
            }
        }

        selected = ownedQueens[bestQueen].gameObject;

        HighlightVisibleSquares(toMove);
        Move(toMove);

        var shots = GetSpacesFrom(toMove);

        bestScope = -1000;
        bestScopesOpponentScope = 1000;
        bestQueen = 0;

        foreach (Vector2Int shot in shots)
        {
            grid[shot.x, shot.y] = 3; //col x , row y

            //left is x and right is y
            Vector2Int scopes = GetScopes();
            int scope = isLeftTurn ? scopes.x : scopes.y;
            int opponentScope = isLeftTurn ? scopes.y : scopes.x;

            grid[shot.x, shot.y] = 0; //col x , row y

            if (minMax)
            {
                if (scope > bestScope || (scope == bestScope && opponentScope < bestScopesOpponentScope))
                {
                    bestQueen = i;
                    bestScope = scope;
                    bestScopesOpponentScope = opponentScope;
                    toShoot = shot;
                }
            }
            else//max/min
            {
                if (opponentScope < bestScopesOpponentScope || (scope == bestScope && scope > bestScope))
                {
                    bestQueen = i;
                    bestScope = scope;
                    bestScopesOpponentScope = opponentScope;
                    toShoot = shot;
                }
            }
        }

        Shoot(toShoot);
    }
    public static void FindRandomTurn()
    {
        List<Vector2Int>[] moves = { new List<Vector2Int>(), new List<Vector2Int>(), new List<Vector2Int>(), new List<Vector2Int>()};
        Queen[] ownedQueens = { null, null, null, null };
        int i = 0;
        foreach (var q in queens)
        {
            if (q.isLeft == isLeftTurn)
            {
                ownedQueens[i] = q;
                moves[i++].AddRange(GetSpacesFrom(q.TranslationToGrid()));
            }
        }

        int randQueen = Random.Range(0, 4);
        while (moves[randQueen].Count == 0)
            randQueen = ++randQueen % 4;

        selected = ownedQueens[randQueen].gameObject;

        toMove = (moves[randQueen][Random.Range(0, moves[randQueen].Count)]);

        HighlightVisibleSquares(toMove);
        Move(toMove);

        var shots = GetSpacesFrom(toMove);
        toShoot = shots[Random.Range(0, shots.Count)];

        Shoot(toShoot);
    }

    void MoveAndShoot()
    {
        bool random = isLeft ? AILeftRandom : AIRightRandom;
        if (random)
            FindRandomTurn();
        else
        {
            gtfo = false;
            if (!FindOptimalTurn(isLeft ? leftDepth : rightDepth, isLeftTurn))
                SwitchToScopeBased();
            else
            {
                HighlightVisibleSquares(toMove);
                Move(toMove);
                Shoot(toShoot);
            }
            foreach (GameObject g in gridSpaces)
            {
                Square s = g.GetComponent<Square>();
                s.isHighlighted = true;
                s.InvertSelected();
            }
        }
    }


    bool waitATurn = true;

    // Update is called once per frame
    void Update()
    {
        //HACKY BAD BAD
        if (!isBoss)
            return;

        if (!(isLeftTurn == isLeft))
            return;

        if (hasWon)
            return;

        if ((isLeft && AILeft) || (!isLeft && AIRight))
        {
            if (!isShooting)
            {
                if(waitATurn)
                {
                    waitATurn = false;
                    GameObject.Find("WorldText").GetComponent<TextMesh>().text = "Thinking...";
                }
                else
                {
                    MoveAndShoot();
                    waitATurn = true;
                }
            }
        }
    }

}
