import gnu.io.CommPort;
import gnu.io.CommPortIdentifier;
import gnu.io.SerialPort;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.FlowLayout;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;
import java.io.InputStream;

import javax.swing.JButton;
import javax.swing.JPanel;

import org.jfree.chart.ChartFactory;
import org.jfree.chart.ChartPanel;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.axis.ValueAxis;
import org.jfree.chart.plot.XYPlot;
import org.jfree.data.time.Millisecond;
import org.jfree.data.time.TimeSeries;
import org.jfree.data.time.TimeSeriesCollection;
import org.jfree.data.xy.XYDataset;
import org.jfree.ui.ApplicationFrame;
import org.jfree.ui.RefineryUtilities; 



class DynamicDataDemo extends ApplicationFrame implements ActionListener {

    /** The time series data. */
    private TimeSeries series1;
    private TimeSeries series2;
    private TimeSeries series3;
    private TimeSeries series4;
    

    /** The most recent value added. */
    private double lastValue = 100.0;

    /**
     * Constructs a new demonstration application.
     *
     * @param title  the frame title.
     */
    @SuppressWarnings("deprecation")
	public DynamicDataDemo(final String title) {

        super(title);
        this.series1 = new TimeSeries(" Data1", Millisecond.class);
        this.series2 = new TimeSeries(" Data2", Millisecond.class);
        this.series3 = new TimeSeries(" Data3", Millisecond.class);
        this.series4 = new TimeSeries(" Data4", Millisecond.class);
        
        final TimeSeriesCollection dataset1 = new TimeSeriesCollection(this.series1);
        final TimeSeriesCollection dataset2 = new TimeSeriesCollection(this.series2);
        final TimeSeriesCollection dataset3 = new TimeSeriesCollection(this.series3);
        final TimeSeriesCollection dataset4 = new TimeSeriesCollection(this.series4);
        	
        // chart name, line color, domain start, domain end
        final JFreeChart chart1 = createChart(dataset1, "branch miss", Color.BLUE, 420.0, 460.0 );	// 0x11
        final JFreeChart chart2 = createChart(dataset2, "data access", Color.cyan,807000.0, 808000.0 );	// 0x06
        final JFreeChart chart3 = createChart(dataset3, "cycle count", Color.magenta, 1441800.0, 1441900.0);	// 0x07
        final JFreeChart chart4 = createChart(dataset4, "data cache miss", Color.RED, 100.0 ,350.0);	// 0x03
            
        final ChartPanel chartPanel1 = new ChartPanel(chart1);
        final ChartPanel chartPanel2 = new ChartPanel(chart2);
        final ChartPanel chartPanel3 = new ChartPanel(chart3);
        final ChartPanel chartPanel4 = new ChartPanel(chart4);
      
        final JButton button = new JButton("Add New Data Item");
        button.setActionCommand("ADD_DATA");
        button.addActionListener(this);

        final JPanel content = new JPanel(new GridLayout(4,1));
        content.add(chartPanel1);
        content.add(chartPanel2);
        content.add(chartPanel3);
        content.add(chartPanel4);
       // content.add(button);
       // button.setSize(500, 70);
        
        chartPanel1.setPreferredSize(new java.awt.Dimension(500, 270));
        chartPanel2.setPreferredSize(new java.awt.Dimension(500, 270));
        chartPanel3.setPreferredSize(new java.awt.Dimension(500, 270));
        chartPanel4.setPreferredSize(new java.awt.Dimension(500, 270));
        setContentPane(content);

    } 
    
    private JFreeChart createChart(final XYDataset dataset, String name, Color color, double start, double end) {
        final JFreeChart result = ChartFactory.createTimeSeriesChart(
            name,
            "Time",
            "Value",
            dataset,
            true,
            true,
            false
        );
        final XYPlot plot = result.getXYPlot();
        plot.getRenderer().setSeriesPaint(0,color);
        ValueAxis axis = plot.getDomainAxis();
        axis.setAutoRange(true);
        axis.setFixedAutoRange(60000.0);  // 60 seconds
        axis = plot.getRangeAxis();
        axis.setRange(start, end);
        return result;
    } 
   
    public void actionPerformed(final ActionEvent e) {
        if (e.getActionCommand().equals("ADD_DATA")) {
            final double factor = 0.90 + 0.2 * Math.random();
            this.lastValue = this.lastValue * factor;
            final Millisecond now = new Millisecond();
   //         System.out.println("Now = " + now.toString());
  //          this.series1.add(new Millisecond(), this.lastValue);
   //         this.series2.add(new Millisecond(), this.lastValue*Math.random());
   //         this.series3.add(new Millisecond(), this.lastValue*Math.random());
   //         this.series4.add(new Millisecond(), this.lastValue*Math.random());
        }
    } 
    
    public void refresh() {

        double factor = 0.90 + 0.2 * Math.random();
        this.lastValue = this.lastValue * factor;
        Millisecond now = new Millisecond();
   //     System.out.println("Now = " + now.toString());
        this.series1.add(new Millisecond(), TwoWaySerialComm.SerialReader.sum[0]);
        this.series2.add(new Millisecond(), TwoWaySerialComm.SerialReader.sum[1]);
        this.series3.add(new Millisecond(), TwoWaySerialComm.SerialReader.sum[2]);
        this.series4.add(new Millisecond(), TwoWaySerialComm.SerialReader.sum[3]);

    }
    
}

public class TwoWaySerialComm {
	public TwoWaySerialComm() {
		super();
	}

	private void connect(String portName) throws Exception {

		
		System.out.printf("Port : %s\n", portName);
		
		CommPortIdentifier portIdentifier = CommPortIdentifier.getPortIdentifier(portName);

		if (portIdentifier.isCurrentlyOwned()) {
			System.out.println("Error: Port is currently in use");
		} else {
			CommPort commPort = portIdentifier.open(this.getClass().getName(), 2000);

			if (commPort instanceof SerialPort) {
				SerialPort serialPort = (SerialPort) commPort;
				serialPort.setSerialPortParams(115200,	// 통신속도
						SerialPort.DATABITS_8, 			// 데이터 비트
						SerialPort.STOPBITS_1,			// stop 비트
						SerialPort.PARITY_NONE);		// 패리티

				// 입력 스트림
				InputStream in = serialPort.getInputStream();
				
				// 출력 스트림
				//OutputStream out = serialPort.getOutputStream();

				(new Thread(new SerialReader(in))).start();
			//	(new Thread(new SerialWriter(out))).start();

			} else {
				System.out
						.println("Error: Only serial ports are handled by this example.");
			}
		}
	}


//	public static class 
	/**
	 * 시리얼 읽기
	 */
	public static class SerialReader implements Runnable {
		InputStream in;

		public static int sum[] = new int[4];
		
		public SerialReader(InputStream in) {
			this.in = in;
		}

		public void run() {
			int data = -1;
			int count = 0;
			int sum_temp[] = new int[4];
			int mycount = 0;
			try {
			
				while(true){
					
				
				while ( (data = this.in.read() ) > -1 ) {
					sum_temp[mycount] += data << (24 - count * 8);
					count++;
				//	System.out.printf("data: %02x / sum: %08x\n",data,sum);
					
			
					if(count == 4){
						//System.out.printf("%08X\n", sum[mycount]);
						count = 0;
						mycount++;
					}
					
					if(mycount == 4)
					{
						
						// call graph function with sum array.
						
						
						for(int i=0;i<4;i++)
						{
							sum[i] = sum_temp[i];
							System.out.printf("%08X\n", sum[i]);
							sum_temp[i] = 0;
						}
						
						System.out.println("------------");
						mycount = 0;
					}
					
				}
				
				}	
				
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}




public static void main(String[] args) {
	try {
		(new TwoWaySerialComm()).connect("COM12");
	} catch (Exception e) {
		e.printStackTrace();
	}
	
	final DynamicDataDemo demo = new DynamicDataDemo("Dynamic Data Demo");
    demo.pack();
    RefineryUtilities.centerFrameOnScreen(demo);
    demo.setVisible(true);
    
    boolean run = true;
    Thread thready = new Thread();
   

    demo.pack();
    RefineryUtilities.centerFrameOnScreen(demo);
    demo.setVisible(true);
   
    thready.start();
    while(run){
         try{
             System.out.println("refresh");
              demo.refresh();
             Thread.sleep(1000);
          }catch(Exception ex){};
    } 
}
}