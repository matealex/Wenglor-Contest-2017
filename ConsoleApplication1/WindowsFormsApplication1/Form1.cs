using System;
using System.Drawing;
using System.IO;
using System.Runtime.InteropServices;
using System.Threading;
using System.Windows.Forms;

namespace WenglorGUIApplication
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        [DllImport("ImageOperations.dll", EntryPoint = "readLogo")]
        public static extern void readLogo();

        [DllImport("ImageOperations.dll", EntryPoint = "processImage")]
        public static extern void processImage(int imageNumber);

        private void button2_Click(object sender, EventArgs e)
        {
            new Thread(() =>
            {
                readLogo();
                var i = 1;
                while (true)
                {
                    try
                    {
                        pictureBox1.Image = Image.FromFile(".\\Input\\input_" + i + ".bmp");
                    }
                    catch (FileNotFoundException)
                    {
                        break;
                    }
                    processImage(i);
                    pictureBox2.Image = Image.FromFile(".\\Output\\output_" + i + ".bmp");
                    i++;
                    Thread.Sleep(3000);
                }

                MessageBox.Show("Done");
            }).Start();
        }
    }
}