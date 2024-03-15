using System;
using System.Windows;

namespace Curculator
{
    /// <summary>
    /// Логика взаимодействия для MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        Calculator calc;
        eMode mode;

        public MainWindow()
        {
            calc = new Calculator();
            mode = eMode.NUMBER;

            InitializeComponent();

            Update();
            UpdateMode();
        }

        private void Update()
        {
            TextBoxExpression.Text = calc.Get() + '_';
        }

        private void UpdateMode()
        {
            switch (mode)
            {
                case eMode.NUMBER:
                    ButtonNum0.Content = "0";
                    ButtonNum1.Content = "1";
                    ButtonNum2.Content = "2";
                    ButtonNum3.Content = "3";
                    ButtonNum4.Content = "4";
                    ButtonNum5.Content = "5";
                    ButtonNum6.Content = "6";
                    ButtonNum7.Content = "7";
                    ButtonNum8.Content = "8";
                    ButtonNum9.Content = "9";
                    ButtonNumA.IsEnabled = true;
                    ButtonNumB.IsEnabled = true;
                    ButtonNumC.IsEnabled = true;
                    ButtonNumD.IsEnabled = true;
                    ButtonNumE.IsEnabled = true;
                    ButtonNumF.IsEnabled = true;
                    ButtonNumG.IsEnabled = true;
                    ButtonNumH.IsEnabled = true;
                    ButtonNumI.IsEnabled = true;
                    ButtonNumJ.IsEnabled = true;
                    ButtonNumK.IsEnabled = true;
                    ButtonNumL.IsEnabled = true;
                    ButtonNumM.IsEnabled = true;
                    ButtonNumN.IsEnabled = true;
                    ButtonNumO.IsEnabled = true;
                    ButtonNumP.IsEnabled = true;
                    ButtonNumQ.IsEnabled = true;
                    ButtonNumR.IsEnabled = true;
                    ButtonNumS.IsEnabled = true;
                    ButtonNumT.IsEnabled = true;
                    ButtonNumU.IsEnabled = true;
                    ButtonNumV.IsEnabled = true;
                    ButtonDot.IsEnabled = true;
                    ButtonAdd.IsEnabled = true;
                    ButtonSub.IsEnabled = true;
                    ButtonMul.IsEnabled = true;
                    ButtonDiv.IsEnabled = true;
                    ButtonOPh.IsEnabled = true;
                    ButtonCPh.IsEnabled = true;
                    break;
                case eMode.NOTATION:
                    ButtonNum0.Content = "₀";
                    ButtonNum1.Content = "₁";
                    ButtonNum2.Content = "₂";
                    ButtonNum3.Content = "₃";
                    ButtonNum4.Content = "₄";
                    ButtonNum5.Content = "₅";
                    ButtonNum6.Content = "₆";
                    ButtonNum7.Content = "₇";
                    ButtonNum8.Content = "₈";
                    ButtonNum9.Content = "₉";
                    ButtonNumA.IsEnabled = false;
                    ButtonNumB.IsEnabled = false;
                    ButtonNumC.IsEnabled = false;
                    ButtonNumD.IsEnabled = false;
                    ButtonNumE.IsEnabled = false;
                    ButtonNumF.IsEnabled = false;
                    ButtonNumG.IsEnabled = false;
                    ButtonNumH.IsEnabled = false;
                    ButtonNumI.IsEnabled = false;
                    ButtonNumJ.IsEnabled = false;
                    ButtonNumK.IsEnabled = false;
                    ButtonNumL.IsEnabled = false;
                    ButtonNumM.IsEnabled = false;
                    ButtonNumN.IsEnabled = false;
                    ButtonNumO.IsEnabled = false;
                    ButtonNumP.IsEnabled = false;
                    ButtonNumQ.IsEnabled = false;
                    ButtonNumR.IsEnabled = false;
                    ButtonNumS.IsEnabled = false;
                    ButtonNumT.IsEnabled = false;
                    ButtonNumU.IsEnabled = false;
                    ButtonNumV.IsEnabled = false;
                    ButtonDot.IsEnabled = false;
                    ButtonAdd.IsEnabled = false;
                    ButtonSub.IsEnabled = false;
                    ButtonMul.IsEnabled = false;
                    ButtonDiv.IsEnabled = false;
                    ButtonOPh.IsEnabled = false;
                    ButtonCPh.IsEnabled = false;
                    break;
            }
        }

        private void NumberFormat()
        {
            if (!(calc.GetPrevSymbol() >= '0' && calc.GetPrevSymbol() <= '9') &&
                !(calc.GetPrevSymbol() >= 'A' && calc.GetPrevSymbol() <= 'V') &&
                calc.GetPrevSymbol() != '.' &&
                calc.GetPrevSymbol() != '\0' &&
                !(calc.GetPrevSymbol() == '-' && !(calc.GetPrevPrevSymbol() >= '0' && calc.GetPrevPrevSymbol() <= '0') && !(calc.GetPrevPrevSymbol() >= '₀' && calc.GetPrevPrevSymbol() <= '₉')))
            {
                calc.AddSymbol(' ');
            }
        }

        private void NotationFormat()
        {
            if (!(calc.GetPrevSymbol() >= '0' && calc.GetPrevSymbol() <= '9') &&
                !(calc.GetPrevSymbol() >= 'A' && calc.GetPrevSymbol() <= 'V') &&
                !(calc.GetPrevSymbol() >= '₀' && calc.GetPrevSymbol() <= '₉') &&
                calc.GetPrevSymbol() != '\0')
            {
                calc.AddSymbol(' ');
            }
        }

        private void DotFormat()
        {
            if (!(calc.GetPrevSymbol() >= '0' && calc.GetPrevSymbol() <= '9') &&
                !(calc.GetPrevSymbol() >= 'A' && calc.GetPrevSymbol() <= 'V') &&
                calc.GetPrevSymbol() != '\0')
            {
                calc.AddSymbol(' ');
            }
        }

        private void OperationFormat()
        {
            calc.AddSymbol(' ');
        }

        private void MultiplyFormat()
        {
            if (calc.GetPrevSymbol() != '*')
            {
                calc.AddSymbol(' ');
            }
        }

        private void ButtonNum1_Click(object sender, RoutedEventArgs e)
        {
            switch (mode)
            {
                case eMode.NUMBER:
                    NumberFormat();
                    calc.AddSymbol('1');
                    break;
                case eMode.NOTATION:
                    NotationFormat();
                    calc.AddSymbol('₁');
                    break;
            }
            Update();
        }

        private void ButtonNum2_Click(object sender, RoutedEventArgs e)
        {
            switch (mode)
            {
                case eMode.NUMBER:
                    NumberFormat();
                    calc.AddSymbol('2');
                    break;
                case eMode.NOTATION:
                    NotationFormat();
                    calc.AddSymbol('₂');
                    break;
            }
            Update();
        }

        private void ButtonNum3_Click(object sender, RoutedEventArgs e)
        {
            switch (mode)
            {
                case eMode.NUMBER:
                    NumberFormat();
                    calc.AddSymbol('3');
                    break;
                case eMode.NOTATION:
                    NotationFormat();
                    calc.AddSymbol('₃');
                    break;
            }
            Update();
        }

        private void ButtonNum4_Click(object sender, RoutedEventArgs e)
        {
            switch (mode)
            {
                case eMode.NUMBER:
                    NumberFormat();
                    calc.AddSymbol('4');
                    break;
                case eMode.NOTATION:
                    NotationFormat();
                    calc.AddSymbol('₄');
                    break;
            }
            Update();
        }

        private void ButtonNum5_Click(object sender, RoutedEventArgs e)
        {
            switch (mode)
            {
                case eMode.NUMBER:
                    NumberFormat();
                    calc.AddSymbol('5');
                    break;
                case eMode.NOTATION:
                    NotationFormat();
                    calc.AddSymbol('₅');
                    break;
            }
            Update();
        }

        private void ButtonNum6_Click(object sender, RoutedEventArgs e)
        {
            switch (mode)
            {
                case eMode.NUMBER:
                    NumberFormat();
                    calc.AddSymbol('6');
                    break;
                case eMode.NOTATION:
                    NotationFormat();
                    calc.AddSymbol('₆');
                    break;
            }
            Update();
        }

        private void ButtonNum7_Click(object sender, RoutedEventArgs e)
        {
            switch (mode)
            {
                case eMode.NUMBER:
                    NumberFormat();
                    calc.AddSymbol('7');
                    break;
                case eMode.NOTATION:
                    NotationFormat();
                    calc.AddSymbol('₇');
                    break;
            }
            Update();
        }

        private void ButtonNum8_Click(object sender, RoutedEventArgs e)
        {
            switch (mode)
            {
                case eMode.NUMBER:
                    NumberFormat();
                    calc.AddSymbol('8');
                    break;
                case eMode.NOTATION:
                    NotationFormat();
                    calc.AddSymbol('₈');
                    break;
            }
            Update();
        }

        private void ButtonNum9_Click(object sender, RoutedEventArgs e)
        {
            switch (mode)
            {
                case eMode.NUMBER:
                    NumberFormat();
                    calc.AddSymbol('9');
                    break;
                case eMode.NOTATION:
                    NotationFormat();
                    calc.AddSymbol('₉');
                    break;
            }
            Update();
        }

        private void ButtonNum0_Click(object sender, RoutedEventArgs e)
        {
            switch (mode)
            {
                case eMode.NUMBER:
                    NumberFormat();
                    calc.AddSymbol('0');
                    break;
                case eMode.NOTATION:
                    NotationFormat();
                    calc.AddSymbol('₀');
                    break;
            }
            Update();
        }

        private void ButtonNumA_Click(object sender, RoutedEventArgs e)
        {
            NumberFormat();
            calc.AddSymbol('A');
            Update();
        }

        private void ButtonNumB_Click(object sender, RoutedEventArgs e)
        {
            NumberFormat();
            calc.AddSymbol('B');
            Update();
        }

        private void ButtonNumC_Click(object sender, RoutedEventArgs e)
        {
            NumberFormat();
            calc.AddSymbol('C');
            Update();
        }

        private void ButtonNumD_Click(object sender, RoutedEventArgs e)
        {
            NumberFormat();
            calc.AddSymbol('D');
            Update();
        }

        private void ButtonNumE_Click(object sender, RoutedEventArgs e)
        {
            NumberFormat();
            calc.AddSymbol('E');
            Update();
        }

        private void ButtonNumF_Click(object sender, RoutedEventArgs e)
        {
            NumberFormat();
            calc.AddSymbol('F');
            Update();
        }

        private void ButtonNumG_Click(object sender, RoutedEventArgs e)
        {
            NumberFormat();
            calc.AddSymbol('G');
            Update();
        }

        private void ButtonNumH_Click(object sender, RoutedEventArgs e)
        {
            NumberFormat();
            calc.AddSymbol('H');
            Update();
        }

        private void ButtonNumI_Click(object sender, RoutedEventArgs e)
        {
            NumberFormat();
            calc.AddSymbol('I');
            Update();
        }

        private void ButtonNumJ_Click(object sender, RoutedEventArgs e)
        {
            NumberFormat();
            calc.AddSymbol('J');
            Update();
        }

        private void ButtonNumK_Click(object sender, RoutedEventArgs e)
        {
            NumberFormat();
            calc.AddSymbol('K');
            Update();
        }

        private void ButtonNumL_Click(object sender, RoutedEventArgs e)
        {
            NumberFormat();
            calc.AddSymbol('L');
            Update();
        }

        private void ButtonNumM_Click(object sender, RoutedEventArgs e)
        {
            NumberFormat();
            calc.AddSymbol('M');
            Update();
        }

        private void ButtonNumN_Click(object sender, RoutedEventArgs e)
        {
            NumberFormat();
            calc.AddSymbol('N');
            Update();
        }

        private void ButtonNumO_Click(object sender, RoutedEventArgs e)
        {
            NumberFormat();
            calc.AddSymbol('O');
            Update();
        }

        private void ButtonNumP_Click(object sender, RoutedEventArgs e)
        {
            NumberFormat();
            calc.AddSymbol('P');
            Update();
        }

        private void ButtonNumQ_Click(object sender, RoutedEventArgs e)
        {
            NumberFormat();
            calc.AddSymbol('Q');
            Update();
        }

        private void ButtonNumR_Click(object sender, RoutedEventArgs e)
        {
            NumberFormat();
            calc.AddSymbol('R');
            Update();
        }

        private void ButtonNumS_Click(object sender, RoutedEventArgs e)
        {
            NumberFormat();
            calc.AddSymbol('S');
            Update();
        }

        private void ButtonNumT_Click(object sender, RoutedEventArgs e)
        {
            NumberFormat();
            calc.AddSymbol('T');
            Update();
        }

        private void ButtonNumU_Click(object sender, RoutedEventArgs e)
        {
            NumberFormat();
            calc.AddSymbol('U');
            Update();
        }

        private void ButtonNumV_Click(object sender, RoutedEventArgs e)
        {
            NumberFormat();
            calc.AddSymbol('V');
            Update();
        }

        private void ButtonDot_Click(object sender, RoutedEventArgs e)
        {
            DotFormat();
            calc.AddSymbol('.');
            Update();
        }

        private void ButtonConv_Click(object sender, RoutedEventArgs e)
        {
            OperationFormat();
            calc.AddSymbol('>');
            Update();
        }

        private void ButtonOPh_Click(object sender, RoutedEventArgs e)
        {
            OperationFormat();
            calc.AddSymbol('(');
            Update();
        }

        private void ButtonCPh_Click(object sender, RoutedEventArgs e)
        {
            OperationFormat();
            calc.AddSymbol(')');
            Update();
        }

        private void ButtonAdd_Click(object sender, RoutedEventArgs e)
        {
            OperationFormat();
            calc.AddSymbol('+');
            Update();
        }

        private void ButtonSub_Click(object sender, RoutedEventArgs e)
        {
            OperationFormat();
            calc.AddSymbol('-');
            Update();
        }

        private void ButtonMul_Click(object sender, RoutedEventArgs e)
        {
            MultiplyFormat();
            calc.AddSymbol('*');
            Update();
        }

        private void ButtonDiv_Click(object sender, RoutedEventArgs e)
        {
            OperationFormat();
            calc.AddSymbol('/');
            Update();
        }

        private void ButtonMode_Click(object sender, RoutedEventArgs e)
        {
            switch (mode)
            {
                case eMode.NUMBER:
                    mode = eMode.NOTATION;
                    break;
                case eMode.NOTATION:
                    mode = eMode.NUMBER;
                    break;
            }
            UpdateMode();
        }

        private void ButtonClear_Click(object sender, RoutedEventArgs e)
        {
            calc.Clear();
            Update();
        }

        private void ButtonResult_Click(object sender, RoutedEventArgs e)
        {
            calc.Evaluate();
            Update();
        }
    }
}
