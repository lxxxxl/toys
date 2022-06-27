using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Drawing.Imaging;
using System.IO;
using System.Configuration;

namespace TextGen
{
    public partial class Form1 : Form
    {
        public int CharHeightPx = 0;
        public int CharWidthPx = 0;
        public int CharOffsetPx = 0;

        public Form1()
        {
            InitializeComponent();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            colorDialog1.ShowDialog();
            button2.ForeColor = colorDialog1.Color;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            String needText = textBox1.Text;
            if (needText.Length == 0)
                return;
            String[] result = needText.Split(new[] { '\r', '\n'}, StringSplitOptions.RemoveEmptyEntries);

            int maxChars = 0;
            for (int i = 0; i < result.Length; i++)
            {
                if (result[i].Length > maxChars)
                    maxChars = result[i].Length;
            }
            int canvasWidth = maxChars * (CharWidthPx + CharOffsetPx);
            int canvasHeight = result.Length * (CharHeightPx + CharOffsetPx);

            Bitmap image = new Bitmap(canvasWidth, canvasHeight);
            Graphics canvas = Graphics.FromImage(image);

            ImageAttributes imageAttributes = new ImageAttributes();
            ColorMap colorMap = new ColorMap();
            colorMap.OldColor = Color.Black;  // opaque red
            colorMap.NewColor = button2.ForeColor;  // opaque blue
            ColorMap[] remapTable = { colorMap };
            imageAttributes.SetRemapTable(remapTable, ColorAdjustType.Bitmap);

            int currentWidth = 0, currentHeight = 0;
            for (int l = 0; l < result.Length; l++)
            {
                for (int c = 0; c < result[l].Length; c++)
                {
                    String filename = "images\\" + result[l][c].ToString() + ".png";
                    if (File.Exists(filename))
                    {
                        Bitmap oneChar = new Bitmap(filename);
                        canvas.DrawImage(
                            oneChar,
                            new Rectangle(currentWidth, currentHeight, CharWidthPx, CharHeightPx),  // destination rectangle 
                            0, 0,        // upper-left corner of source rectangle 
                            oneChar.Width,       // width of source rectangle
                            oneChar.Height,      // height of source rectangle
                            GraphicsUnit.Pixel,
                            imageAttributes);
                    }
                    currentWidth += (CharWidthPx + CharOffsetPx);
                }
                currentWidth = 0;
                currentHeight += (CharHeightPx + CharOffsetPx);
            }

            canvas.Save();
            SaveFileDialog d = new SaveFileDialog();
            d.AddExtension = true;
            d.DefaultExt = "png";
            d.ShowDialog();
            if (d.FileName != String.Empty)
                image.Save(d.FileName);

            System.Diagnostics.Process.Start(d.FileName);
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            CharHeightPx = int.Parse(ConfigurationManager.AppSettings["CharHeightPx"]);
            CharWidthPx = int.Parse(ConfigurationManager.AppSettings["CharWidthPx"]);
            CharOffsetPx = int.Parse(ConfigurationManager.AppSettings["CharOffsetPx"]);
        }
    }
}
