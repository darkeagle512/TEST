import javax.swing.*;
import java.awt.*;

public class SimpleTest extends JFrame {
    public SimpleTest() {
        setTitle("Simple Java Test");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setSize(400, 300);
        setLocationRelativeTo(null);
        
        JLabel label = new JLabel("Java is working!", JLabel.CENTER);
        label.setFont(new Font("Arial", Font.BOLD, 24));
        add(label);
        
        setVisible(true);
    }
    
    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> new SimpleTest());
    }
}
