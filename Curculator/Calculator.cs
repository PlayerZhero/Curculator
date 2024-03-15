using System;
using System.Runtime.InteropServices;

namespace Curculator
{
    class Calculator
    {
#if DEBUG
        [DllImport("C:\\Users\\PlayerZhero\\source\\repos\\Curculator\\Debug\\libCurculator.dll", CallingConvention = CallingConvention.StdCall)]
#else
        [DllImport("libCurculator.dll", CallingConvention = CallingConvention.StdCall)]
#endif
        [return: MarshalAs(UnmanagedType.LPWStr)]
        static extern string calculate([MarshalAs(UnmanagedType.LPWStr)] string str);

        string line;

        public Calculator()
        {
            Clear();
        }

        public void AddSymbol(char c)
        {
            line += c;
        }

        public char GetPrevSymbol()
        {
            if (line.Length > 0) return line[line.Length - 1];

            return '\0';
        }

        public char GetPrevPrevSymbol()
        {
            int id = line.LastIndexOf(' ');

            if (id != -1 && id > 0)
            {
                return line[id - 1];
            }

            return '\0';
        }

        public void Clear()
        {
            line = "";
        }

        public void Delete()
        {
            for (int i = line.Length - 2; i >= 0; i--)
            {
                if (line[i] != ' ' && line[i] != '\n')
                {
                    line = line.Substring(0, i + 1);
                    return;
                }
            }

            line = "";
        }

        public string Get()
        {
            return line;
        }

        public void Evaluate()
        {
            line = calculate(line);
        }
    }
}
