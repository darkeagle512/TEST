import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.awt.geom.*;

public class AnimatedSphere extends JFrame {
    public AnimatedSphere() {
        setTitle("Animated Sphere");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setSize(800, 600);
        setLocationRelativeTo(null);
        setResizable(false);
        
        SpherePanel panel = new SpherePanel();
        add(panel);
        
        setVisible(true);
    }
    
    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> new AnimatedSphere());
    }
}

class SpherePanel extends JPanel {
    private float rotationX = 0;
    private float rotationY = 0;
    private float rotationZ = 0;
    
    private static final int SPHERE_RADIUS = 100;
    private static final int SECTORS = 30;
    private static final int STACKS = 30;
    
    private Point3D[] vertices;
    private Face[] faces;
    
    public SpherePanel() {
        setBackground(new Color(20, 20, 20));
        
        // Generate sphere mesh
        generateSphere();
        
        // Start animation thread
        new Thread(() -> {
            while (true) {
                rotationX += 0.005f;
                rotationY += 0.007f;
                rotationZ += 0.003f;
                
                repaint();
                
                try {
                    Thread.sleep(16); // ~60 FPS
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
            }
        }).start();
    }
    
    private void generateSphere() {
        java.util.List<Point3D> vertexList = new java.util.ArrayList<>();
        java.util.List<Face> faceList = new java.util.ArrayList<>();
        
        float sectorStep = 2 * (float)Math.PI / SECTORS;
        float stackStep = (float)Math.PI / STACKS;
        
        // Generate vertices
        for (int i = 0; i <= STACKS; i++) {
            float stackAngle = (float)Math.PI / 2 - i * stackStep;
            float xy = SPHERE_RADIUS * (float)Math.cos(stackAngle);
            float z = SPHERE_RADIUS * (float)Math.sin(stackAngle);
            
            for (int j = 0; j <= SECTORS; j++) {
                float sectorAngle = j * sectorStep;
                
                float x = xy * (float)Math.cos(sectorAngle);
                float y = xy * (float)Math.sin(sectorAngle);
                
                vertexList.add(new Point3D(x, y, z));
            }
        }
        
        // Generate faces
        for (int i = 0; i < STACKS; i++) {
            int k1 = i * (SECTORS + 1);
            int k2 = k1 + SECTORS + 1;
            
            for (int j = 0; j < SECTORS; j++, k1++, k2++) {
                if (i != 0) {
                    faceList.add(new Face(k1, k2, k1 + 1));
                }
                
                if (i != (STACKS - 1)) {
                    faceList.add(new Face(k1 + 1, k2, k2 + 1));
                }
            }
        }
        
        vertices = vertexList.toArray(new Point3D[0]);
        faces = faceList.toArray(new Face[0]);
    }
    
    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);
        Graphics2D g2d = (Graphics2D) g;
        g2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
        
        int width = getWidth();
        int height = getHeight();
        
        // Transform and project vertices
        Point3D[] transformedVertices = new Point3D[vertices.length];
        Point2D[] projectedVertices = new Point2D[vertices.length];
        
        for (int i = 0; i < vertices.length; i++) {
            // Rotate
            Point3D v = vertices[i];
            Point3D rotated = rotatePoint(v, rotationX, rotationY, rotationZ);
            transformedVertices[i] = rotated;
            
            // Project to 2D
            float scale = 300 / (rotated.z + 400);
            float x = rotated.x * scale + width / 2.0f;
            float y = -rotated.y * scale + height / 2.0f;
            projectedVertices[i] = new Point2D(x, y);
        }
        
        // Sort faces by average Z (painter's algorithm)
        java.util.List<FaceWithDepth> facesWithDepth = new java.util.ArrayList<>();
        for (Face face : faces) {
            float avgZ = (transformedVertices[face.v1].z + 
                         transformedVertices[face.v2].z + 
                         transformedVertices[face.v3].z) / 3.0f;
            facesWithDepth.add(new FaceWithDepth(face, avgZ));
        }
        
        facesWithDepth.sort((a, b) -> Float.compare(a.depth, b.depth));
        
        // Draw faces
        for (FaceWithDepth faceWithDepth : facesWithDepth) {
            Face face = faceWithDepth.face;
            Point2D p1 = projectedVertices[face.v1];
            Point2D p2 = projectedVertices[face.v2];
            Point2D p3 = projectedVertices[face.v3];
            
            // Calculate brightness based on normal
            float brightness = calculateBrightness(
                transformedVertices[face.v1],
                transformedVertices[face.v2],
                transformedVertices[face.v3]
            );
            
            brightness = Math.max(0.2f, brightness);
            
            int colorValue = (int)(brightness * 200);
            Color color = new Color(colorValue / 255.0f, colorValue / 255.0f * 0.8f, 1.0f);
            g2d.setColor(color);
            
            // Draw triangle
            Path2D path = new Path2D.Float();
            path.moveTo(p1.x, p1.y);
            path.lineTo(p2.x, p2.y);
            path.lineTo(p3.x, p3.y);
            path.closePath();
            
            g2d.fillPath(path);
            
            // Draw outline
            g2d.setColor(new Color(100, 100, 150));
            g2d.setStroke(new BasicStroke(0.5f));
            g2d.drawPath(path);
        }
        
        // Draw info
        g2d.setColor(Color.WHITE);
        g2d.setFont(new Font("Arial", Font.PLAIN, 14));
        g2d.drawString("Animated Java Sphere", 10, 20);
        g2d.drawString("Rotation: X=" + String.format("%.2f", rotationX) + 
                      " Y=" + String.format("%.2f", rotationY) + 
                      " Z=" + String.format("%.2f", rotationZ), 10, 40);
    }
    
    private Point3D rotatePoint(Point3D p, float angleX, float angleY, float angleZ) {
        // Rotate around X axis
        float y1 = (float)(p.y * Math.cos(angleX) - p.z * Math.sin(angleX));
        float z1 = (float)(p.y * Math.sin(angleX) + p.z * Math.cos(angleX));
        
        // Rotate around Y axis
        float x2 = (float)(p.x * Math.cos(angleY) + z1 * Math.sin(angleY));
        float z2 = (float)(-p.x * Math.sin(angleY) + z1 * Math.cos(angleY));
        
        // Rotate around Z axis
        float x3 = (float)(x2 * Math.cos(angleZ) - y1 * Math.sin(angleZ));
        float y3 = (float)(x2 * Math.sin(angleZ) + y1 * Math.cos(angleZ));
        
        return new Point3D(x3, y3, z2);
    }
    
    private float calculateBrightness(Point3D v1, Point3D v2, Point3D v3) {
        // Calculate face normal
        Point3D edge1 = new Point3D(v2.x - v1.x, v2.y - v1.y, v2.z - v1.z);
        Point3D edge2 = new Point3D(v3.x - v1.x, v3.y - v1.y, v3.z - v1.z);
        
        // Cross product
        Point3D normal = new Point3D(
            edge1.y * edge2.z - edge1.z * edge2.y,
            edge1.z * edge2.x - edge1.x * edge2.z,
            edge1.x * edge2.y - edge1.y * edge2.x
        );
        
        // Normalize
        float length = (float)Math.sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
        if (length > 0) {
            normal.x /= length;
            normal.y /= length;
            normal.z /= length;
        }
        
        // Light direction (from camera)
        Point3D lightDir = new Point3D(0, 0, 1);
        
        // Dot product for brightness
        float brightness = Math.max(0, normal.x * lightDir.x + normal.y * lightDir.y + normal.z * lightDir.z);
        
        return brightness;
    }
    
    static class Point3D {
        float x, y, z;
        
        Point3D(float x, float y, float z) {
            this.x = x;
            this.y = y;
            this.z = z;
        }
    }
    
    static class Point2D {
        float x, y;
        
        Point2D(float x, float y) {
            this.x = x;
            this.y = y;
        }
    }
    
    static class Face {
        int v1, v2, v3;
        
        Face(int v1, int v2, int v3) {
            this.v1 = v1;
            this.v2 = v2;
            this.v3 = v3;
        }
    }
    
    static class FaceWithDepth {
        Face face;
        float depth;
        
        FaceWithDepth(Face face, float depth) {
            this.face = face;
            this.depth = depth;
        }
    }
}
