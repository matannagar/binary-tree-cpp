#ifndef NUMBERWITHUNITS_H
#define NUMBERWITHUNITS_H
#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <stdexcept>
using namespace std;

namespace ariel
{

    struct Graph
    {
        vector<vector<pair<string, vector<pair<string, double>>>>> matrix;
        unordered_map<string, uint> umap;
        unordered_map<string, uint> lomap;
    };

    static vector<string> units;
    static Graph graph;

    class NumberWithUnits
    {

    public:
        string unit;
        double num;
        NumberWithUnits(double num, string unit);
        
        NumberWithUnits();

        double getNum();
        void setNum(double x);
        string getUnit();

        static void read_units(std::ifstream &file);

        friend NumberWithUnits operator+(const NumberWithUnits &f1, const NumberWithUnits &f2);
        friend NumberWithUnits operator+=(NumberWithUnits &f1, const NumberWithUnits &f2);
        friend NumberWithUnits operator+(const NumberWithUnits &f1);

        friend NumberWithUnits operator-(const NumberWithUnits &f1, const NumberWithUnits &f2);
        friend NumberWithUnits operator-=(NumberWithUnits &f1, const NumberWithUnits &f2);
        friend NumberWithUnits operator-(const NumberWithUnits &f1);

        bool operator<(const NumberWithUnits &f) const;
        bool operator<=(const NumberWithUnits &f) const;
        bool operator>(const NumberWithUnits &f) const;
        bool operator>=(const NumberWithUnits &f) const;
        bool operator==(const NumberWithUnits &f) const;
        bool operator!=(const NumberWithUnits &f) const;

        NumberWithUnits &operator++();         // prefix: ++a
        const NumberWithUnits operator++(int); // postfix: a++

        NumberWithUnits &operator--();         // prefix: --a
        const NumberWithUnits operator--(int); // postfix: a--

        friend NumberWithUnits operator*(const NumberWithUnits &f1, const double &f2);
        friend NumberWithUnits operator*(const double &f1, const NumberWithUnits &f2);

        friend std::ostream &operator<<(std::ostream &os, const NumberWithUnits &f);
        friend std::istream &operator>>(std::istream &is, NumberWithUnits &f);

        static void initGraph(vector<string> &units, Graph &graph);
        static void print(const Graph &graph);
        static uint findIndex(Graph &graph, string unit);
        static double connect(Graph &graph, string left_unit, std::string right_unit);
        static void insert2units(vector<string> &units, Graph &graph, uint i);
        static void insert1unit(vector<string> &units, Graph &graph, uint i);
    };
}

#endif