
namespace WinFormPractice
{
    partial class Form1
    {
        /// <summary>
        /// 필수 디자이너 변수입니다.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 사용 중인 모든 리소스를 정리합니다.
        /// </summary>
        /// <param name="disposing">관리되는 리소스를 삭제해야 하면 true이고, 그렇지 않으면 false입니다.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form 디자이너에서 생성한 코드

        /// <summary>
        /// 디자이너 지원에 필요한 메서드입니다. 
        /// 이 메서드의 내용을 코드 편집기로 수정하지 마세요.
        /// </summary>
        private void InitializeComponent()
        {
            this.test_label1 = new System.Windows.Forms.Label();
            this.ttBtn = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // test_label1
            // 
            this.test_label1.AutoSize = true;
            this.test_label1.Location = new System.Drawing.Point(427, 112);
            this.test_label1.Name = "test_label1";
            this.test_label1.Size = new System.Drawing.Size(64, 15);
            this.test_label1.TabIndex = 0;
            this.test_label1.Text = "TestText";
            this.test_label1.Click += new System.EventHandler(this.label1_Click);
            // 
            // ttBtn
            // 
            this.ttBtn.Location = new System.Drawing.Point(267, 104);
            this.ttBtn.Name = "ttBtn";
            this.ttBtn.Size = new System.Drawing.Size(75, 23);
            this.ttBtn.TabIndex = 1;
            this.ttBtn.Text = "button1";
            this.ttBtn.UseVisualStyleBackColor = true;
            this.ttBtn.Click += new System.EventHandler(this.button1_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 450);
            this.Controls.Add(this.ttBtn);
            this.Controls.Add(this.test_label1);
            this.Name = "Form1";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label test_label1;
        private System.Windows.Forms.Button ttBtn;
    }
}

