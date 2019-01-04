using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using ZedGraph;

namespace USBSpeedTest
{
    public partial class ADForm : Form
    {

        public ADForm()
        {
            InitializeComponent();

            Data.MyPane = zedGraphControl1.GraphPane;
            Data.MyPane.Title = "AD显示表";
            Data.MyPane.XAxis.Title = "时间";
            Data.MyPane.YAxis.Title = "值";
            double[] x = new double[100];
            double[] y = new double[100];
            for (int i = 0; i < 1; i++)
            {
                x[i] = 0;
                y[i] = 0;
            }

            if (Data.MyPane.CurveList != null)
                Data.MyPane.CurveList.Clear();

            Data.MyPane.AddCurve("AD1", x, y, Color.Red, SymbolType.Square);
            Data.MyPane.AddCurve("AD2", x, y, Color.Gold, SymbolType.Square);
            Data.MyPane.AddCurve("AD3", x, y, Color.Green, SymbolType.Square);
            Data.MyPane.AddCurve("AD4", x, y, Color.Blue, SymbolType.Square);
            Data.MyPane.AddCurve("AD5", x, y, Color.Red, SymbolType.Square);
            Data.MyPane.AddCurve("AD6", x, y, Color.Gold, SymbolType.Square);
            Data.MyPane.AddCurve("AD7", x, y, Color.Green, SymbolType.Square);
            Data.MyPane.AddCurve("AD8", x, y, Color.Blue, SymbolType.Square);
            Data.MyPane.AddCurve("AD9", x, y, Color.Red, SymbolType.Square);
            Data.MyPane.AddCurve("AD10", x, y, Color.Gold, SymbolType.Square);
            Data.MyPane.AddCurve("AD11", x, y, Color.Green, SymbolType.Square);
            Data.MyPane.AddCurve("AD12", x, y, Color.Blue, SymbolType.Square);
            Data.MyPane.AddCurve("AD13", x, y, Color.Red, SymbolType.Square);
            Data.MyPane.AddCurve("AD14", x, y, Color.Gold, SymbolType.Square);
            Data.MyPane.AddCurve("AD15", x, y, Color.Green, SymbolType.Square);
            Data.MyPane.AddCurve("AD16", x, y, Color.Blue, SymbolType.Square);

        }

        public void Init_Table()
        {

            dataGridView1.DataSource = Data.dt_AD01;
            dataGridView1.AllowUserToAddRows = false;


            dataGridView2.DataSource = Data.dt_AD02;
            dataGridView2.AllowUserToAddRows = false;
        }

        private void ADForm_Load(object sender, EventArgs e)
        {
            Data.AdFrmIsAlive = true;
            Init_Table();
            Data.PaneCount = 0;
            //       zedGraphControl1.GraphPane.Title = "AD显示表";



        }

        private void ADForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            Data.AdFrmIsAlive = false;
            this.Dispose();
            zedGraphControl1.Dispose();
        }



        private void timer1_Tick(object sender, EventArgs e)
        {
            if (Data.AdFrmIsAlive)
            {
                zedGraphControl1.AxisChange();
                zedGraphControl1.Invalidate();
            }
        }
    }
}
