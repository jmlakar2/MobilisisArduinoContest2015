namespace safe_client
{
    partial class SafeClient
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.connBtn = new System.Windows.Forms.Button();
            this.discBtn = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // connBtn
            // 
            this.connBtn.Location = new System.Drawing.Point(38, 127);
            this.connBtn.Name = "connBtn";
            this.connBtn.Size = new System.Drawing.Size(75, 23);
            this.connBtn.TabIndex = 0;
            this.connBtn.Text = "Connect";
            this.connBtn.UseVisualStyleBackColor = true;
            this.connBtn.Click += new System.EventHandler(this.connBtn_Click);
            // 
            // discBtn
            // 
            this.discBtn.Enabled = false;
            this.discBtn.Location = new System.Drawing.Point(142, 126);
            this.discBtn.Name = "discBtn";
            this.discBtn.Size = new System.Drawing.Size(75, 23);
            this.discBtn.TabIndex = 1;
            this.discBtn.Text = "Disconnect";
            this.discBtn.UseVisualStyleBackColor = true;
            this.discBtn.Click += new System.EventHandler(this.button2_Click);
            // 
            // SafeClient
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(284, 262);
            this.Controls.Add(this.discBtn);
            this.Controls.Add(this.connBtn);
            this.Name = "SafeClient";
            this.Text = "Form1";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button connBtn;
        private System.Windows.Forms.Button discBtn;
    }
}

