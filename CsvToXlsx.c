using Microsoft.Office.Interop.Excel;
using OfficeOpenXml;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

//===================================================
// �X�V����
//===================================================
// 2024/06/20 : 012020048D : �V�K�ǉ�
// 2024/06/21 : 012020048D : Task3 : csv_to_xlsx_w_split() �ǉ�
// 2024/06/21 : 012020048D : �����e�X�g_M
//===================================================




internal static class CsvToXlsx
{
    //===================================================
    // CSV��XLSX�ϊ��@�\�i�t�@�C����������j
    // file_path : �Ώۃt�@�C���̃p�X
    // folder_path : �o�͐�t�H���_�̃p�X
    //===================================================
    static public void csv_to_xlsx_w_split(string file_path, string folder_path)
    {
        // �o�͗p�t�@�C���iCSV�j
        string output_csv = "����-";
        string output_csv_name;

        // �o�͗p�t�@�C���iExcel�j
        string output_xlsx_name;
        string output_sheet_name = "�o�̓V�[�g";

        string Ex_line; // ���o�s�i���H�ρj
        string Ex_line_mem = "dummy"; // ���o�s�i�O��L���l�j

        string line_1st; // ���o�s�̐擪����
        string line_1st_U = "�w�b�_";
        string line_1st_mem = "dummy"; // �����z���p�L���l

        string blank_line = "\0";
        string blank_data = "";
        string split_sign = ",";
        string end_line = "���|�[�g���b�Z�[�W";

        // �e�X�g
        int char_max = 1000;         // �t�@�C���쐬���
        long len_max = 1000000;     // ���C�����
        long line_limit = 950000;   // �t�@�C���؂�ւ�臒l

        bool header_flg = false;
        int header_judge = 32;

        int row_cnt;

        // �s�����擾(�w�b�_�[���܂܂�)
        string[] lines = File.ReadAllLines(file_path, Encoding.GetEncoding("Shift_JIS"));
        long cntRow = lines.Length - 1;

        // �������̌���
        long file_max_L = cntRow / len_max;   // ��(�؎�)
                                              //int file_max = Convert.ToInt32(file_max_L) + 1;
        int file_max = char_max;

        // �t�@�C���Ǎ�
        using (var sr = new System.IO.StreamReader(file_path, Encoding.GetEncoding("Shift_JIS")))
        {

            // CSV�쐬(�V�K�쐬)
            for (int file_num = 1; file_num <= file_max; file_num++)
            {
                output_csv_name = output_csv + line_1st_U + "-" + file_num + ".csv";
                output_xlsx_name = output_csv + line_1st_U + "-" + file_num + ".xlsx";

                //�o�͐�p�X�̍쐬
                var move_path = folder_path + @"\" + output_xlsx_name;

                using (var output_xlsx = new ExcelPackage())
                //using (var sw = new System.IO.StreamWriter(output_csv_name, false, System.Text.Encoding.GetEncoding("shift_jis")))
                {

                    // var outputSheet = output_xlsx.Workbook.Worksheets[0];
                    var outputSheet = output_xlsx.Workbook.Worksheets.Add(output_sheet_name);
                    row_cnt = 1;

                    // �O��L���l�̏�������
                    if (Ex_line_mem != "dummy")
                    {
                        // ��������
                        //sw.WriteLine(Ex_line_mem);

                        // CSV�f�[�^�̕����i�[�iline�̕�����,�ŋ�؂�j
                        string[] Split_data_mem = Ex_line_mem.Split(',');

                        Ex_line_mem = "dummy";

                        // Excel��������
                        for (int data_num = 0; data_num < Split_data_mem.Length; data_num++)
                        {
                            outputSheet.Cells[row_cnt, (data_num + 1)].Value = Split_data_mem[data_num];

                        }
                        row_cnt++;

                    }

                    // �񐔕����[�v
                    for (int line_cnt = 0; line_cnt < len_max; line_cnt++)
                    {
                        // 1�s�ǂݏo��
                        Ex_line = sr.ReadLine();

                        // �ǂ݂������s���󔒂ł͂Ȃ��ꍇ
                        if (Ex_line != null)
                        {

                            // CSV�f�[�^�̕����i�[�iline�̕�����,�ŋ�؂�j
                            string[] Split_data = Ex_line.Split(',');

                            // ���j�^�p
                            var split_moni_0 = Split_data[0];

                            // �w�b�_�i�ݒ�I�v�V�������j�̏�������
                            if ((line_cnt < header_judge) && (header_flg == false))
                            {
                                // ��������
                                //sw.WriteLine(Ex_line);

                                // Excel��������
                                for (int data_num = 0; data_num < Split_data.Length; data_num++)
                                {
                                    outputSheet.Cells[row_cnt, (data_num + 1)].Value = Split_data[data_num];


                                }

                                row_cnt++;

                            }
                            else if (Ex_line.Contains(end_line))
                            {

                                file_max = file_num;
                                break;
                            }
                            // �{�̕����̏�������
                            else
                            {
                                // �w�b�_�t���O��OFF����
                                if (header_flg == false)
                                {

                                    header_flg = true;

                                }
                                // 1��ڂ��󔒁A�܂��́A��s�����݂̂̏ꍇ
                                if ((Split_data[0] == blank_line)
                                    || (Split_data[0] == blank_data))
                                {

                                    // ��������
                                    //sw.WriteLine(Ex_line);

                                    // Excel��������
                                    for (int data_num = 0; data_num < Split_data.Length; data_num++)
                                    {
                                        outputSheet.Cells[row_cnt, (data_num + 1)].Value = Split_data[data_num];

                                    }

                                    row_cnt++;

                                }
                                //  1��ڂɏ�񂪂���ꍇ
                                else if (Split_data[0] != blank_line)
                                {
                                    // �擪�����̎擾�i�啶���j
                                    line_1st = Convert.ToString(Ex_line.FirstOrDefault());
                                    line_1st_U = line_1st.ToUpper();


                                    // �擪�������O��l�ƈ�v�A���A1�t�@�C���̏o�͏�������̏ꍇ
                                    if ((line_1st != split_sign)
                                        && (line_cnt < line_limit)
                                        && ((line_1st_U == line_1st_mem)
                                            || (line_1st_U == "dummy")))
                                    {
                                        // ��������
                                        //sw.WriteLine(Ex_line);

                                        // Excel��������
                                        for (int data_num = 0; data_num < Split_data.Length; data_num++)
                                        {
                                            outputSheet.Cells[row_cnt, (data_num + 1)].Value = Split_data[data_num];

                                        }
                                        row_cnt++;
                                    }
                                    else if ((line_cnt >= line_limit)
                                            || ((line_1st_U != line_1st_mem)
                                                || (line_1st_U != blank_line)
                                                || (line_1st_U != split_sign)))
                                    {
                                        Ex_line_mem = Ex_line;
                                        line_1st_mem = line_1st_U;

                                        var fileInfo = new FileInfo(output_xlsx_name);
                                        output_xlsx.SaveAs(fileInfo);

                                        //�o�̓t�@�C���̈ړ�
                                        if (System.IO.Directory.Exists(folder_path) == true)
                                        {

                                            System.IO.File.Move(output_xlsx_name, move_path);

                                        }

                                        break;
                                    }
                                    else
                                    {
                                        // ��������
                                        //sw.WriteLine(Ex_line);

                                        // Excel��������
                                        for (int data_num = 0; data_num < Split_data.Length; data_num++)
                                        {
                                            outputSheet.Cells[row_cnt, (data_num + 1)].Value = Split_data[data_num];

                                        }
                                        row_cnt++;
                                    }

                                }
                                else
                                {

                                    // ��������
                                    //sw.WriteLine(Ex_line);

                                    // Excel��������
                                    for (int data_num = 0; data_num < Split_data.Length; data_num++)
                                    {
                                        outputSheet.Cells[row_cnt, (data_num + 1)].Value = Split_data[data_num];

                                    }
                                    row_cnt++;

                                }
                            }
                        }
                        // �ǂ݂������s���󔒂̏ꍇ 
                        else
                        {
                            var fileInfo = new FileInfo(output_xlsx_name);
                            output_xlsx.SaveAs(fileInfo);

                            //�o�̓t�@�C���̈ړ�
                            if (System.IO.Directory.Exists(folder_path) == true)
                            {

                                System.IO.File.Move(output_xlsx_name, move_path);

                            }

                            break;
                        }
                    }

                    output_xlsx?.Dispose();
                }

            }

            // �t�@�C�������
            sr?.Close();
        }

        return;
    }

}

